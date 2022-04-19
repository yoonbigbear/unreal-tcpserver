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
                //이미 계정이 있음.
                if (res.next())
                {
                    if (!session)
                    {
                        LOG_ERROR("Session Error id:{} pw:{}", login_id, login_password);
                        return;
                    }

                    // accout_id는 서버에서만 알도록 하며 세션에 저장해둔다.
                    session->account_id(res.get<int>("acct_id"));

                    // 로그인 성공했으니 캐릭터 현황을 받아온다.
                    exist = DB::select_characters(session->account_id(), res);

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
                                static_cast<char>(res.get<uint16_t>("class")),
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
        }
        catch (std::exception e)
        {
            LOG_CRITICAL("DB Failed select account");
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

        if (!session)
        {
            LOG_ERROR("session error id:{}", nickname);
            return;
        }

        int ret = -1;
        try
        {
            // 1 을 강제로 넣는데, 로그인할때 유저세션이 가지고 있도록 수정해야한다.
            ret = DB::create_character(id, 1, nickname, class_id);
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

        try
        {
            auto ret = DB::select_character_nickname(nickname);
            if (!session)
            {
                std::cout << "lose session" << std::endl;
            }
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

void CheckNickname(session::Shared session, message msg)
{
}
