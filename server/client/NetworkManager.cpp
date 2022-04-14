#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>

void PacketTest(CustomClient& c)
{
    //CreateAccount Pkt test
    //{
    //    BUILD_PACKET(CreateAccountReq, "sampleId", "samplePassword");
    //    c.Send(pkt);

    //    WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateAccountReq);

    //    auto body = flatbuffers::GetRoot<account::CreateAccountAck>(msg.msg.body.data());
    //    std::cout << "pkt received:" << body->result() << std::endl;

    //    WAIT_UNTIL_END;
    //}

    //login account
    {
        BUILD_PACKET(LoginReq, "sampleId", "samplePassword");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_LoginAck);

        auto body = flatbuffers::GetRoot<account::LoginAck>(msg.msg.body.data());
        std::cout << "pkt received:" << body->result() << std::endl;

        WAIT_UNTIL_END;
    }
}
