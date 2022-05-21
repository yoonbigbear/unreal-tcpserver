#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "pch.h"

#include <server_interface.h>

#include <protocol_generated.h>

#include "network/packet/packet_manager.h"
#include "network/packet/account/account_packets.h"
#include <protocol_generated.h>

#include "world/world_manager.h"

using namespace boost;

namespace net
{
    class CustomServer : public ServerInterface
    {
    public:
        CustomServer(uint16_t port) : ServerInterface(port)
        {
            AccountPackets temp;
            temp.Start();

            WorldManager::instance().Start();
        }

    protected:
        virtual bool OnClientConnect(SessionPtr client) override
        {
            std::cout << "클라 세션 붙었음" << std::endl;

            return true;
        }

        virtual void OnClientDisconnect(SessionPtr client) override
        {
            std::cout << "Removing client [" << client << "]" << std::endl;
        }

        virtual void OnMessage(SessionPtr session, Packet& msg) override
        {
            auto func = PacketManager::instance().packet_handler(static_cast<Protocol>(msg.id));
            if (func)
                func(session, msg);
            else
                LOG_ERROR("No Callback Func protocol:{}", EnumNameProtocol(static_cast<Protocol>(msg.id)));
            
        }
    };

} // namespace net

#endif // !_NET_SERVER_H_
