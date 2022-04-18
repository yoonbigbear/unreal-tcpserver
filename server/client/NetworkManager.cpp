#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>

void PacketTest(CustomClient& c)
{
    //CreateAccount Pkt test
    /*{
        BUILD_PACKET(CreateAccountReq, "sampleId", "samplePassword");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateAccountReq);

        auto body = flatbuffers::GetRoot<account::CreateAccountAck>(msg.msg.body.data());
        std::cout << "pkt received:" << body->result() << std::endl;

        WAIT_UNTIL_END;
    }*/

    //login account
    {
        BUILD_PACKET(LoginReq, "sampleId", "samplePassword");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_LoginAck);

        auto body = flatbuffers::GetRoot<account::LoginAck>(msg.msg.body.data());
        LOG_INFO("result {}", body->result());
        for (int i = 0; i < body->characters()->size(); ++i)
        {
            auto info = body->characters()->Get(i);
            auto nickname = info->nickname()->str();
            auto job = info->class_();
            LOG_INFO("character nickname {} job {}", nickname, job);
        }


        WAIT_UNTIL_END;
    }

    //create character
    /*{
        BUILD_PACKET(CreateCharacterReq, "sampleCharacter", 1);
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateCharacterAck);

        auto body = flatbuffers::GetRoot<account::CreateCharacterAck>(msg.msg.body.data());
        DEBUG_LOG_INFO("result %d", body->result());

        WAIT_UNTIL_END;
    }*/
}
