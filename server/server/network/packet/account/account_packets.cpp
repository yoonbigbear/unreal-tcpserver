#include "account_packets.h"

#include "../packet_manager.h"

#include "macro.h"
#include "database/DB.h"

#include <protocol_generated.h>
#include <common_generated.h>
#include <account_generated.h>
#include <result_code_generated.h>

#include "network/client_session.h"
#include "world/world_manager.h"

#include "snowflake.hpp"

#include "object/character.h"

using snowflake_t = net::snowflake<1534832906275L>;


void CreateAccount(session::Shared session, message& msg);
void LoginAccount(session::Shared session, message& msg);

void CreateCharacter(session::Shared session, message msg);
void CheckNickname(session::Shared session, message msg);
void SelectCharacter(session::Shared session, message msg);

void AccountPackets::Start()
{
    PacketManager::instance().Bind(Protocol_CreateAccountReq, CreateAccount);
    PacketManager::instance().Bind(Protocol_LoginReq, LoginAccount);
    PacketManager::instance().Bind(Protocol_CreateCharacterReq, CreateCharacter);
    PacketManager::instance().Bind(Protocol_CheckCharacterNicknameReq, CheckNickname);
    PacketManager::instance().Bind(Protocol_SelectCharacterReq, SelectCharacter);
}

void CreateAccount(session::Shared session, message& msg)
{
    auto pkt = flatbuffers::GetRoot<account::CreateAccountReq>(msg.body.data());
    auto login_id = pkt->id()->str();
    auto login_password = pkt->pw()->str();

    asio::post([session, login_id, login_password]() {

        if (!session)
        {
            LOG_ERROR("lose session");
            return;
        }

        try
        {
            auto ret = DB::create_account(login_id, login_password);

            if (ret == 0)
            {
                BUILD_PACKET(CreateAccountAck, ResultCode_LoginSuccess);
                session->Send(pkt);
            }
            else
            {
                BUILD_PACKET(CreateAccountAck, ResultCode_AleadyExist);
                session->Send(pkt);
            }

            DEBUG_LOG_INFO("Account Create Success");
        }
        catch (std::exception e)
        {
            LOG_ERROR("Failed create account: {}", e.what());
        }

        });
}

void LoginAccount(session::Shared session, message& msg)
{
    auto pkt = flatbuffers::GetRoot<account::LoginReq>(msg.body.data());
    auto login_id = pkt->id()->str();
    auto login_password = pkt->pw()->str();

    asio::post([session, login_id, login_password]() {
        nanodbc::result res;

        try
        {
            auto exist = DB::select_account(login_id, login_password, res);
            if (exist == 1)
            {
                //�̹� ������ ����.
                if (res.next())
                {
                    if (!session)
                    {
                        LOG_ERROR("Session Error id:{} pw:{}", login_id, login_password);
                        return;
                    }

                    // accout_id�� ���������� �˵��� �ϸ� ���ǿ� �����صд�.
                    session->acct_id(res.get<int>("acct_id"));
                    exist = -1;
                    // �α��� ���������� ĳ���� ��Ȳ�� �޾ƿ´�.
                    exist = DB::select_characters(session->acct_id(), res);

                    if (exist == 1)
                    {
                        //ĳ���� ���� ������ Ȯ�� �� ĳ���� �������� ���������
                        net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                        pkt.header.id = Protocol_LoginAck;

                        flatbuffers::FlatBufferBuilder fbb(1024);
                        std::vector<flatbuffers::Offset<CharacterInfo>> character_infos;

                        if (res.rowset_size() == 0)
                        {
                            //ĳ���� ����
                            net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                            pkt.header.id = Protocol_LoginAck;
                            flatbuffers::FlatBufferBuilder fbb(1024);
                            auto builder = account::LoginAckBuilder(fbb);
                            builder.add_result(ResultCode_LoginSuccess);
                            auto fin = builder.Finish();
                            fbb.Finish(fin);
                            pkt << fbb;
                            session->Send(pkt);

                            DEBUG_LOG_INFO("���� ���� ����. ĳ���� ����");
                            return;
                        }

                        while (res.next())
                        {
                            auto nickname = fbb.CreateString(res.get<std::string>("nickname"));
                            character_infos.emplace_back(CreateCharacterInfo(fbb,
                                static_cast<char>(res.get<uint16_t>("class")),
                                res.get<uint64_t>("char_id"),
                                nickname));
                            auto characters = fbb.CreateVector(character_infos);

                            auto builder = account::LoginAckBuilder(fbb);
                            builder.add_result(ResultCode_LoginSuccess);
                            builder.add_characters(characters);
                            auto fin = builder.Finish();
                            fbb.Finish(fin);
                            pkt << fbb;
                            session->Send(pkt);

                            DEBUG_LOG_INFO("���� ���� ����. ĳ���� ����.");
                            return;
                        }
                    }
                }

                LOG_CRITICAL("select_account failed");

            }
            else
            {
                //���� ���̵� �̰ų� Ʋ�� ��й�ȣ
                net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                pkt.header.id = Protocol_LoginAck;
                flatbuffers::FlatBufferBuilder fbb(1024);
                auto builder = account::LoginAckBuilder(fbb);
                builder.add_result(ResultCode_LoginFailure);
                auto fin = builder.Finish();
                fbb.Finish(fin);
                pkt << fbb;
                session->Send(pkt);
            }
        }
        catch (std::exception e)
        {
            LOG_CRITICAL("DB Failed select account");
        }
        });

}


void CreateCharacter(session::Shared session, message msg)
{
    //id�� ���� �������� �ϴµ� �ϴ��� �׳� ����
    snowflake_t uuid;
    uuid.init(1, 1);
    auto id = uuid.nextid();

    auto pkt = flatbuffers::GetRoot<account::CreateCharacterReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();
    auto class_id = pkt->job_class();

    asio::post([session, nickname, class_id, id]() {

        if (!session)
        {
            LOG_ERROR("session error id:{}", nickname);
            return;
        }

        int ret = -1;
        try
        {
            // 1 �� ������ �ִµ�, �α����Ҷ� ���������� ������ �ֵ��� �����ؾ��Ѵ�.
            ret = DB::create_character(id, session->acct_id(), nickname, class_id);
        }
        catch (std::exception e)
        {
            LOG_ERROR("Failed create character: {}", e.what());
            return;
        }

        if (ret == 0)
        {
            net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
            pkt.header.id = Protocol_CreateCharacterAck;
            flatbuffers::FlatBufferBuilder fbb(1024);
            auto builder = account::LoginAckBuilder(fbb);
            builder.add_result(ResultCode_CreateSuccess);
            auto fin = builder.Finish();
            fbb.Finish(fin);
            pkt << fbb;
            session->Send(pkt);
        }
        else
        {
            net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
            pkt.header.id = Protocol_CreateCharacterAck;
            flatbuffers::FlatBufferBuilder fbb(1024);
            auto builder = account::LoginAckBuilder(fbb);
            builder.add_result(ResultCode_CreateFailure);
            auto fin = builder.Finish();
            fbb.Finish(fin);
            pkt << fbb;
            session->Send(pkt);
        }
        });
}

void CheckNickname(session::Shared session, message msg)
{
    auto pkt = flatbuffers::GetRoot<account::CheckCharacterNicknameReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();

    asio::post([session, nickname]() {

        try
        {
            if (!session)
            {
                std::cout << "lose session" << std::endl;
                return;
            }

            auto ret = DB::select_character_nickname(nickname);
            if (ret == 0)
            {
                net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                pkt.header.id = Protocol_CheckCharacterNicknameAck;
                flatbuffers::FlatBufferBuilder fbb(1024);
                auto builder = account::LoginAckBuilder(fbb);
                builder.add_result(ResultCode_NotExist);
                auto fin = builder.Finish();
                fbb.Finish(fin);
                pkt << fbb;
                session->Send(pkt);

            }
            else
            {
                net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                pkt.header.id = Protocol_CheckCharacterNicknameAck;
                flatbuffers::FlatBufferBuilder fbb(1024);
                auto builder = account::LoginAckBuilder(fbb);
                builder.add_result(ResultCode_AleadyExist);
                auto fin = builder.Finish();
                fbb.Finish(fin);
                pkt << fbb;
                session->Send(pkt);
            }
        }
        catch (std::exception e)
        {
            LOG_ERROR("Failed create character: {}", e.what());
        }
        });
}

void SelectCharacter(session::Shared session, message msg)
{
    //Req ��Ŷ ����
    auto pkt = flatbuffers::GetRoot<account::SelectCharacterReq>(msg.body.data());
    auto char_id = pkt->char_id();

    asio::post([session, char_id] {

        if (!session)
        {
            LOG_ERROR("lose session");
            return;
        }

        try
        {
            nanodbc::result res;
            DB::select_character(session->acct_id(), char_id, res);

            if (res.next())
            {
                Vec3 pos(res.get<float>("pos_x"), res.get<float>("pos_y"), res.get<float>("pos_z"));

                net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                pkt.header.id = Protocol_SelectCharacterAck;
                flatbuffers::FlatBufferBuilder fbb(1024);
                auto builder = account::SelectCharacterAckBuilder(fbb);
                builder.add_result(ResultCode_EnterGameSuccess);
                builder.add_position(&pos);
                auto fin = builder.Finish();
                fbb.Finish(fin);
                pkt << fbb;
                session->Send(pkt);

                //ĳ���� ���� ����. ���ӿ�����Ʈ ����
                {
                    CharacterPtr character = std::make_shared<Character>(static_cast<uint32_t>(char_id), 1);
                    character->session(session);
                    character->start();
                    WorldManager::instance().EnterField(0, character);
                }
            }
            else
            {
                // LOG_CRITICAL("���� ĳ���� ������ ����. acct_id:{} char_id:{}", session->acct_id(), char_id);
                Vec3 pos(0, 0, 0);

                net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                pkt.header.id = Protocol_SelectCharacterAck;
                flatbuffers::FlatBufferBuilder fbb(1024);
                auto builder = account::SelectCharacterAckBuilder(fbb);
                builder.add_result(ResultCode_EnterGameSuccess);
                builder.add_position(&pos);
                auto fin = builder.Finish();
                fbb.Finish(fin);
                pkt << fbb;
                session->Send(pkt);
                return;
            }
        }
        catch (std::exception e)
        {
            LOG_CRITICAL("Failed create character: {}", e.what());
        }

        });
}