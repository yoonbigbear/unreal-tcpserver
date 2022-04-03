#ifndef _SAMPLE_PACKET_HANDLER_H_

#include <unordered_map>
#include <functional>

#include <message.h>

#include "../../network/protocol.h"
#include "../../network/client_session.h"

class SamplePacketHandler
{
public:
    SamplePacketHandler();

    static void CreateCharacter(ClientSession::Shared session, net::Message<protocol>& msg);
    static void SelectCharacterNickname(ClientSession::Shared session, net::Message<protocol>& msg);

    static void CreateAccount(ClientSession::Shared session, net::Message<protocol>& msg);
    static void LoginAccount(ClientSession::Shared session, net::Message<protocol>& msg);

    static void SendText(ClientSession::Shared session, net::Message<protocol>& msg);

private:
    std::unordered_map<protocol,
        std::function<void(ClientSession::Shared, net::Message<protocol>&)>> packet_handler_;
}; // class SamplerPacketHandler

#endif // !_SAMPLE_PACKET_HANDLER_H_

