#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include "object/character.h"
#include <protocol_generated.h>

template<typename T = Protocol, typename U = flatbuffer>
class ClientSession : public net::Session<T, U>
{
public:
    using Shared = std::shared_ptr<ClientSession<T, U>>;
    using Weak = std::weak_ptr<ClientSession<T, U>>;

    ClientSession() = delete;
    ClientSession(net::Session<T, U>::owner parent, asio::io_context& io_context, asio::ip::tcp::socket&& socket, net::PacketQueue<net::OwnedMessage<T, U>>& in)
        : net::Session<T, U>(parent, io_context, std::move(socket), in) {}

    bool EnterCharacter(uint64_t char_id, int char_class)
    {
        if (!character_)
        {
            LOG_ERROR("이미 캐릭터가 있으면 안된다.");
            return;
        }

        character_ = std::make_shared<Character>(char_id, char_class);
    }

    int acct_id() { return acct_id_; }
    void acct_id(int id) { acct_id_ = id; }


private:
    int acct_id_ = 0;
    Character::Shared character_;

}; // class ClientSession

#endif // !_CLIENT_SESSION_H_