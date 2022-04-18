#include "account_packets.h"

#include "../packet_manager.h"

#include "macro.h"
#include "database/DB.h"
#include <protocol_generated.h>
#include <common_generated.h>
#include <account_generated.h>
#include <result_code_generated.h>

#include "snowflake.hpp"

using snowflake_t = snowflake<1534832906275L>;


void CreateAccount(session::Shared session, message& msg);
void LoginAccount(session::Shared session, message& msg);

void CreateCharacter(session::Shared session, message msg);
void SelectCharacterNickname(session::Shared session, message msg);
void CheckNickname(session::Shared session, message msg);

void AccountPackets::Start()
{
    PacketManager::instance().Bind(Protocol_CreateAccountReq, CreateAccount);
    PacketManager::instance().Bind(Protocol_LoginReq, LoginAccount);
    PacketManager::instance().Bind(Protocol_CreateCharacterReq, CreateCharacter);
    PacketManager::instance().Bind(Protocol_CheckCharacterNicknameReq, SelectCharacterNickname);
}

void CreateAccount(session::Shared session, message& msg)
{
    auto pkt = flatbuffers::GetRoot<account::CreateAccountReq>(msg.body.data());
    auto account_id = pkt->id()->str();
    auto account_password = pkt->pw()->str();

    asio::post([session, account_id, account_password]() {

        auto ret = DB::create_account(account_id, account_password);

        if (!session)
        {
            std::cout << "lose session" << std::endl;
        }
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
        });
}

void LoginAccount(session::Shared session, message& msg)
{
    auto pkt = flatbuffers::GetRoot<account::LoginReq>(msg.body.data());
    auto account_id = pkt->id()->str();
    auto account_password = pkt->pw()->str();

    asio::post([session, account_id, account_password]() {
        nanodbc::result res;
        auto exist = DB::select_account(account_id, account_password, res);
        if (exist == 1)
        {
            if (res.next())
            {
                auto account_id = res.get<uint64_t>("acct_id");

                exist = DB::select_characters(account_id, res);

                if (exist > 0)
                {
                    //캐릭터 숫자 몇인지 확인 후 캐릭터 정보까지 보내줘야함
                    net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                    pkt.header.id = Protocol_LoginAck;

                    flatbuffers::FlatBufferBuilder fbb(1024);
                    std::vector<flatbuffers::Offset<CharacterInfo>> character_infos;
                    while (res.next())
                    {
                        auto nickname = fbb.CreateString(res.get<std::string>("nickname"));
                        character_infos.emplace_back(CreateCharacterInfo(fbb,
                            res.get<uint16_t>("class"),
                            nickname));
                    }
                    auto characters = fbb.CreateVector(character_infos);

                    auto builder = account::LoginAckBuilder(fbb);
                    builder.add_result(ResultCode_LoginSuccess);
                    builder.add_characters(characters);
                    auto fin = builder.Finish();
                    fbb.Finish(fin);
                    pkt << fbb;
                    session->Send(pkt);

                    return;
                }
                else
                {
                    //캐릭터 없음
                    net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
                    pkt.header.id = Protocol_LoginAck;
                    flatbuffers::FlatBufferBuilder fbb(1024);
                    auto builder = account::LoginAckBuilder(fbb);
                    builder.add_result(ResultCode_LoginSuccess);
                    auto fin = builder.Finish();
                    fbb.Finish(fin);
                    pkt << fbb;
                    session->Send(pkt);

                    return;
                }
                
            }

            LOG_CRITICAL("select_account failed");

        }
        else
        {
           //없는 아이디 이거나 틀린 비밀번호
            net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
            pkt.header.id = Protocol_LoginAck;
            flatbuffers::FlatBufferBuilder fbb(1024);
            auto builder = account::LoginAckBuilder(fbb);
            builder.add_result(ResultCode_LoginFailed);
            auto fin = builder.Finish();
            fbb.Finish(fin);
            pkt << fbb;
            session->Send(pkt);
        }
        });
}


void CreateCharacter(session::Shared session, message msg)
{
    //id를 새로 만들어줘야 하는데 일단은 그냥 하자
    snowflake_t uuid;
    uuid.init(1, 1);
    auto id = uuid.nextid();

    auto pkt = flatbuffers::GetRoot<account::CreateCharacterReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();
    auto class_id = pkt->class_();

    asio::post([session, nickname, class_id, id]() {

        int ret = -1;
        try
        {
            ret = DB::create_character(id, 1, nickname, class_id);
        }
        catch (std::exception e)
        {

        }

        if (!session)
        {
            std::cout << "lose session" << std::endl;
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
            builder.add_result(ResultCode_CreateFailed);
            auto fin = builder.Finish();
            fbb.Finish(fin);
            pkt << fbb;
            session->Send(pkt);
        }
        });
}

void SelectCharacterNickname(session::Shared session, message msg)
{
    auto pkt = flatbuffers::GetRoot<account::CheckCharacterNicknameReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();

    asio::post([session, nickname]() {
        auto ret = DB::select_character_nickname(nickname);
        if (!session)
        {
            std::cout << "lose session" << std::endl;
        }
        if (ret == 0)
        {
            message msg;
            //SamplePacketHelper::ResultCodeAck(msg, ResultCode_NotExist);
            session->Send(msg);

        }
        else
        {
            message msg;
            // SamplePacketHelper::ResultCodeAck(msg, ResultCode_AleadyExist);
            session->Send(msg);
        }
        });
}

void CheckNickname(session::Shared session, message msg)
{
}
