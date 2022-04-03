#include "samplepacket_handler.h"

#include <boost/asio.hpp>
#include <SamplePacket_generated.h>

#include "snowflake.hpp"

#include "../../database/DB.h"
#include "samplepacket_helper.h"

using snowflake_t = net::snowflake<1534832906275L>;

SamplePacketHandler::SamplePacketHandler()
{
    packet_handler_.try_emplace(protocol::kCreateAccount, CreateAccount);
    packet_handler_.try_emplace(protocol::kLogin, LoginAccount);
    packet_handler_.try_emplace(protocol::kTextSend, SendText);

}

void SamplePacketHandler::CreateCharacter(ClientSession::Shared session, net::Message<protocol>& msg)
{
    //id를 새로 만들어줘야 하는데 일단은 그냥 하자
    snowflake_t uuid;
    uuid.init(1, 1);
    auto id = uuid.nextid();
    
    auto pkt = flatbuffers::GetRoot<SamplePacket::CreateCharacterReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();
    auto class_id = pkt->class_();

   boost::asio::post([session, nickname, class_id, id]() {

        auto ret = DB::create_character(id, nickname, class_id);
        if (!session)
        {
            std::cout << "lose session" << std::endl;
        }
        if (ret == 0)
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kCreateSuccess);
            session->Send(msg);
        }
        else
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kCreateFailed);
            session->Send(msg);
        }
   });
}

void SamplePacketHandler::SelectCharacterNickname(ClientSession::Shared session, net::Message<protocol>& msg)
{
    auto pkt = flatbuffers::GetRoot<SamplePacket::SelectCharacterNickNameReq>(msg.body.data());
    auto nickname = pkt->nickname()->str();

    boost::asio::post([session, nickname]() {
        auto ret = DB::select_character_nickname(nickname);
        if (!session)
        {
            std::cout << "lose session" << std::endl;
        }
        if (ret == 0)
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kNotExist);
            session->Send(msg);

        }
        else
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kAleadyExist);
            session->Send(msg);
        }
    });
}

void SamplePacketHandler::CreateAccount(ClientSession::Shared session, net::Message<protocol>& msg)
{
    auto pkt = flatbuffers::GetRoot<SamplePacket::LoginReq>(msg.body.data());
    auto account_id = pkt->id()->str();
    auto account_password = pkt->pw()->str();

    boost::asio::post([session, account_id, account_password]() {

        auto ret = DB::create_account(account_id, account_password);

        if (!session)
        {
            std::cout << "lose session" << std::endl;
        }
        if (ret == 0)
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kLoginSuccess);
            session->Send(msg);

        }
        else
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kAleadyExist);
            session->Send(msg);
        }
    });

}

void SamplePacketHandler::LoginAccount(ClientSession::Shared session, net::Message<protocol>& msg)
{
    auto pkt = flatbuffers::GetRoot<SamplePacket::LoginReq>(msg.body.data());
    auto account_id = pkt->id()->str();
    auto account_password = pkt->pw()->str();

    boost::asio::post([session, account_id, account_password]() {
        auto exist = DB::select_account(account_id, account_password);
        if (exist == 0)
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kLoginSuccess);
            session->Send(msg);
        }
        else
        {
            net::Message<protocol> msg;
            SamplePacketHelper::ResultCodeAck(msg, result_code::kLoginFailed);
            session->Send(msg);
        }
    });
}

void SamplePacketHandler::SendText(ClientSession::Shared session, net::Message<protocol>& msg)
{
    auto text = flatbuffers::GetRoot<SamplePacket::textREQ>(msg.body.data());
    DB::add_chat(text->text()->c_str());
}
