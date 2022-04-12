#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>

void PacketTest(CustomClient& c)
{
    //CreateAccount Pkt test
    {
        SEND_REQ(CreateAccount, "sampleId", "samplePassword");
        c.Send(msg);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateAccount_Ack);

        auto body = flatbuffers::GetRoot<account::CreateAccountAck>(msg.msg.body.data());
        std::cout << "pky received:" << body->result() << std::endl;

        WAIT_UNTIL_END;
    }
}
