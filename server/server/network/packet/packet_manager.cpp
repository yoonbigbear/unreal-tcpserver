#include "packet_manager.h"

bool PacketManager::Bind(Protocol protocol, std::function<void(session::Shared, message&)> callback)
{
    if (packet_handler_.contains(protocol))
        return false;

    packet_handler_.try_emplace(protocol, callback);
    return true;
}