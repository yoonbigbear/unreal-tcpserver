#ifndef _PACKET_MANAGER_
#define _PACKET_MANAGER_

#include <unordered_map>
#include <functional>

#include <singleton.h>

#include "network/client_session.h"

using func = std::function<void(std::shared_ptr<Session>, Packet&)>;

class PacketManager : public net::Singleton<PacketManager>
{
public:

    bool Bind(Protocol protocol, std::function<void(std::shared_ptr<Session>, Packet&)> callback);

    auto packet_handler(Protocol protocol) { return packet_handler_.contains(protocol) ? packet_handler_[protocol] : nullptr; }

private:
    std::unordered_map<Protocol, std::function<void(std::shared_ptr<Session>, Packet&)>> packet_handler_;
};

#endif / /_PACKET_MANAGER_