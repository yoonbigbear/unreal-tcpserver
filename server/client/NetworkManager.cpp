#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>

void TestLogic(CustomClient& c)
{
    {
        SEND_REQ(CreateAccount, "sampleId", "samplePassword");
        //net::Message<Protocol, flatbuffers::FlatBufferBuilder> msg;
        //msg.header.id = Protocol_CreateAccount_Req;
        //flatbuffers::FlatBufferBuilder builder(1024);
        //auto res = account::CreateCreateAccountReqDirect(builder, "sampleId", "samplePassword");
        //builder.Finish(res);
        //msg << builder;
        c.Send(msg);
    }
}
