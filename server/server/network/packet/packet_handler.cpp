#include "packet_handler.h"

bool PacketHandler::bind(Protocol protocol, func callback)
{
    if (packet_handler_.contains(protocol))
        return false;

    packet_handler_.try_emplace(protocol, callback);
    return true;
}
