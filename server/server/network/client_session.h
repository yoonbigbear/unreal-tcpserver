#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include <boost/asio.hpp>

#include <session.h>
#include "protocol.h"
#include "packet_queue.h"

#include "../user/character.h"

using namespace boost;

class ClientSession : public net::Session<protocol>
{
public:
    ClientSession(owner parent, asio::io_context& io_context, asio::ip::tcp::socket&& socket, net::PacketQueue<net::OwnedMessage<protocol>>& in)
        : net::Session<protocol>(parent, io_context, std::move(socket), in) {}

    using Shared = std::shared_ptr<ClientSession>;
    using Weak = std::weak_ptr<ClientSession>;

private:
    Character::Shared character_;

}; // class ClientSession

#endif // !_CLIENT_SESSION_H_
