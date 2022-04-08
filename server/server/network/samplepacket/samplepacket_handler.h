#ifndef _SAMPLE_PACKET_HANDLER_H_

#include <unordered_map>
#include <functional>

#include "network/client_session.h"

#include <protocol_generated.h>

class SamplePacketHandler
{
public:
   
    using message = net::Message<Protocol, flatbuffer>;
    using session = ClientSession<Protocol, flatbuffer>;
    using func = std::function<void(session::Shared, message&)> ;
    SamplePacketHandler();

    static void CreateCharacter(session::Shared session, message& msg);
    static void SelectCharacterNickname(session::Shared session, message& msg);

    static void CreateAccount(session::Shared session, message& msg);
    static void LoginAccount(session::Shared session, message& msg);

    static void CheckNickname(session::Shared session, message& msg);


private:
    std::unordered_map<Protocol, func> packet_handler_;
}; // class SamplerPacketHandler

#endif // !_SAMPLE_PACKET_HANDLER_H_

