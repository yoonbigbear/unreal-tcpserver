#ifndef _CLIENT_SESSION_H_
#define _CLIENT_SESSION_H_

#include "object/character.h"
#include "pch.h"

#include "../world/world_manager.h"

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

    bool CreateCharacter(uint64_t char_id, int char_class)
    {
        if (character_)
        {
            LOG_ERROR("이미 캐릭터가 있으면 안된다.");
            return false;
        }

        if (acct_id_ == 0)
        {
            LOG_ERROR("Account id가 아직 세팅되지 않았다.");
            return false;
        }

        character_ = std::make_shared<Character>(char_id, char_class);

        return true;
    }


    virtual void Disconnect() override
    {
        net::Session<T, U>::Disconnect();
        if (character_)
        {
            auto field = WorldManager::instance().field(character_->map_id());
            field->Leave(character_->obj_id());
        }
    }


    int acct_id() { return acct_id_; }
    void acct_id(int id) { acct_id_ = id; }

    CharacterPtr character() { return character_; }

private:
    int acct_id_ = 0;
    CharacterPtr character_ = nullptr;
}; // class ClientSession
#endif // !_CLIENT_SESSION_H_