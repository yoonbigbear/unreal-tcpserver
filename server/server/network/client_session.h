#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include "pch.h"

#include <protocol_generated.h>

using namespace net;

template<typename T = Protocol, typename U = flatbuffers::FlatBufferBuilder>
class ClientSession : public net::Session<T, U>
{
public:
    using Shared = std::shared_ptr<ClientSession<T, U>>;
    using Weak = std::weak_ptr<ClientSession<T, U>>;

    ClientSession() = delete;
    ClientSession(net::Session<T, U>::owner parent, asio::io_context& io_context, asio::ip::tcp::socket&& socket, net::PacketQueue<net::OwnedMessage<T, U>>& in)
        : net::Session<T, U>(parent, io_context, std::move(socket), in) {}

    virtual void Disconnect() override
    {
        net::Session<T, U>::Disconnect();
    }


    int acct_id() { return acct_id_; }
    void acct_id(int id) { acct_id_ = id; }


private:
    int acct_id_ = 0;


}; // class ClientSession

using SessionPtr = ClientSession<Protocol, flatbuffer>::Shared;
#endif // !_CLIENT_SESSION_H_