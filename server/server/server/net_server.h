#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "pch.h"

#include <server_interface.h>

#include <protocol_generated.h>

#include "network/client_session.h"
#include "network/packet/packet_manager.h"
#include "network/packet/account/account_packets.h"
#include <protocol_generated.h>

#include "world/world_manager.h"

using namespace boost;

namespace net
{
    template<typename T = ClientSession<Protocol, flatbuffers::FlatBufferBuilder>>
    class CustomServer : public ServerInterface<Protocol, T>
    {
    public:
        CustomServer(uint16_t port) : ServerInterface<Protocol, ClientSession<Protocol, flatbuffers::FlatBufferBuilder>>(port)
        {
            AccountPackets temp;
            temp.Start();

            WorldManager::instance().Start();
        }

    protected:
        virtual bool OnClientConnect(std::shared_ptr<T> client) override
        {
            std::cout << "클라 세션 붙었음" << std::endl;

            return true;
        }

        virtual void OnClientDisconnect(std::shared_ptr<T> client) override
        {
            std::cout << "Removing client [" << client << "]" << std::endl;
        }

        virtual void OnMessage(std::shared_ptr<T> session, Message<Protocol, flatbuffer>& msg) override
        {
            auto func = PacketManager::instance().packet_handler(msg.header.id);
            if (func)
                func(session, msg);
            else
                LOG_ERROR("No Callback Func protocol:{}", EnumNameProtocol(msg.header.id));

        }
    };

} // namespace net

#endif // !_NET_SERVER_H_
