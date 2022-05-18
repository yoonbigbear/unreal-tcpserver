#ifndef _PACKET_H_
#define _PACKET_H_

#include <iostream>
#include <boost/asio.hpp>

namespace net
{
    struct Packet
    {
        uint16_t id = 0;
        uint16_t size = 0;
        std::vector<uint8_t> body;
    };

    struct PacketSession
    {
        Packet packet_;
        std::shared_ptr<Session> pacekt_owner_;
    };

} // namespace net

#endif // _PACKET_H_