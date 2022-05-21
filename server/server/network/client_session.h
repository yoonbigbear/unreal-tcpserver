#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include "pch.h"

#include <protocol_generated.h>

using namespace net;

class ClientSession : public net::Session
{
public:
    ClientSession() = delete;
    ClientSession(net::Session::owner parent, asio::io_context& io_context,
        asio::ip::tcp::socket&& socket, net::PacketQueue<PacketSession>& in)
        : net::Session(parent, io_context, std::move(socket), in) {}

    virtual void Disconnect() override
    {
        net::Session::Disconnect();
    }

private:


}; // class ClientSession

#endif // !_CLIENT_SESSION_H_