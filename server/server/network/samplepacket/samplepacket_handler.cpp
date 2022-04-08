#include "samplepacket_handler.h"

#include "pch.h"

#include "snowflake.hpp"

#include "database/DB.h"
#include "samplepacket_helper.h"

#include <account_generated.h>
#include <protocol_generated.h>
#include <result_code_generated.h>

using snowflake_t = snowflake<1534832906275L>;

SamplePacketHandler::SamplePacketHandler()
{
    packet_handler_.try_emplace(Protocol_Login_Req, LoginAccount);
    packet_handler_.try_emplace(Protocol_CreateAccount_Req, CreateAccount);
    packet_handler_.try_emplace(Protocol_CheckCharacterNickname_Req, CheckNickname);
    packet_handler_.try_emplace(Protocol_CreateCharacter_Req, CreateCharacter);

}

void SamplePacketHandler::CreateCharacter(session::Shared session, message& msg)
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
           ret = DB::create_character(id, nickname, class_id);
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
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_CreateSuccess);
            session->Send(msg);
        }
        else
        {
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_CreateFailed);
            session->Send(msg);
        }
   });
}

void SamplePacketHandler::SelectCharacterNickname(session::Shared session, message& msg)
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
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_NotExist);
            session->Send(msg);

        }
        else
        {
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_AleadyExist);
            session->Send(msg);
        }
    });
}

void SamplePacketHandler::CreateAccount(session::Shared session, message& msg)
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
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_LoginSuccess);
            session->Send(msg);

        }
        else
        {
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_AleadyExist);
            session->Send(msg);
        }
    });

}

void SamplePacketHandler::LoginAccount(session::Shared session, message& msg)
{
    auto pkt = flatbuffers::GetRoot<account::LoginReq>(msg.body.data());
    auto account_id = pkt->id()->str();
    auto account_password = pkt->pw()->str();

    asio::post([session, account_id, account_password]() {
        auto exist = DB::select_account(account_id, account_password);
        if (exist == 0)
        {
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_LoginSuccess);
            session->Send(msg);
        }
        else
        {
            message msg;
            SamplePacketHelper::ResultCodeAck(msg, ResultCode_LoginFailed);
            session->Send(msg);
        }
    });
}

void SamplePacketHandler::CheckNickname(session::Shared session, message& msg)
{
}
