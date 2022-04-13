#include "account_manager.h"
#include <functional>

#include "macro.h"
#include "database/DB.h"

#include <protocol_generated.h>
#include <account_generated.h>
#include <result_code_generated.h>

void AccountManager::Start()
{
    std::function<void(PacketHandler::session::Shared, PacketHandler::message&)> creaete_account_callback =
        [](auto session, auto msg) {
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
                SEND_ACK(CreateAccount, ResultCode_LoginSuccess);
                session->Send(msg);

            }
            else
            {
                SEND_ACK(CreateAccount, ResultCode_AleadyExist);
                session->Send(msg);
            }
            });
    };

    PacketHandler::instance().bind(Protocol_CreateAccount_Req, creaete_account_callback);
}

void AccountManager::CreateAccountAck(PacketHandler::session::Shared session, PacketHandler::message& msg)
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
            SEND_ACK(CreateAccount, ResultCode_LoginSuccess);
            session->Send(msg);

        }
        else
        {
            SEND_ACK(CreateAccount, ResultCode_AleadyExist);
            session->Send(msg);
        }
        });
}
