#ifndef _PACKET_H_
#define _PACKET_H_

#include <iostream>
#include <boost/asio.hpp>

namespace net
{
    //����� �������� id�� body�������� ����� ����.
    struct PacketHeader
    {
        uint16_t id = 0;
        uint32_t bodysize = 0;
    };

    struct Packet
    {
        PacketHeader header;
        std::vector<uint8_t> body;
    };
} // namespace net

#endif // _PACKET_H_