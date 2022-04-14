#include "account_packets.h"

#include "../packet_manager.h"

#include "macro.h"
#include "database/DB.h"
#include <protocol_generated.h>
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
            //ĳ���� ���� ������ Ȯ�� �� ĳ���� ���ڱ��� ���������
            
            //BUILD_PACKET(LoginAck, ResultCode_LoginSuccess);
            //session->Send(pkt);
        }
        else
        {
           // BUILD_PACKET(LoginAck, ResultCode_LoginFailed);
            //session->Send(pkt);
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
            //SamplePacketHelper::ResultCodeAck(msg, ResultCode_CreateSuccess);
            session->Send(msg);
        }
        else
        {
            message msg;
            //SamplePacketHelper::ResultCodeAck(msg, ResultCode_CreateFailed);
            session->Send(msg);
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
