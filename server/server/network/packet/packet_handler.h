#ifndef _PACKET_HANDLER_
#define _PACKET_HANDLER_

#include <unordered_map>
#include <functional>

#include <singleton.h>

#include "network/client_session.h"

class PacketHandler : public net::Singleton<PacketHandler>
{
public:

    using message = net::Message<Protocol, flatbuffer>;
    using session = ClientSession<Protocol, flatbuffer>;
    using func = std::function<void(session::Shared, message&)>;

    bool bind(Protocol protocol, std::function<void(session::Shared, message&)> callback);

private:
    std::unordered_map<Protocol, std::function<void(session::Shared, message&)>> packet_handler_;
};

#endif