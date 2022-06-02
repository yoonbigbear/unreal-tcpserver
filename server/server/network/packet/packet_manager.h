#ifndef _PACKET_MANAGER_
#define _PACKET_MANAGER_

#include <unordered_map>
#include <functional>

#include <singleton.h>

#include "network/client_session.h"

using message = net::Message<Protocol, flatbuffer>;
using session = ClientSession<Protocol, flatbuffer>;
using func = std::function<void(session::Shared, message&)>;

class PacketManager : public net::Singleton<PacketManager>
{
public:

    bool Bind(Protocol protocol, std::function<void(session::Shared, message&)> callback);

    auto packet_handler(Protocol protocol) { return packet_handler_.contains(protocol) ? packet_handler_[protocol] : nullptr; }

private:
    std::unordered_map<Protocol, std::function<void(session::Shared, message&)>> packet_handler_;
};

#endif / /_PACKET_MANAGER_