#include "packet_handler.h"

bool PacketHandler::bind(Protocol protocol, std::function<void(session::Shared, message&)> callback)
{
    if (packet_handler_.contains(protocol))
        return false;

    packet_handler_.try_emplace(protocol, callback);
    return true;
}