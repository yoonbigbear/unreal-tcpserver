#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "pch.h"

#include <server_interface.h>

#include <protocol_generated.h>

#include "network/client_session.h"
#include "network/samplepacket/samplepacket_handler.h"

using namespace boost;

namespace net
{
    template<typename T = ClientSession<Protocol, flatbuffers::FlatBufferBuilder>>
    class CustomServer : public ServerInterface<Protocol, T>
    {
    public:
        CustomServer(uint16_t port) : ServerInterface<Protocol, ClientSession<Protocol, flatbuffers::FlatBufferBuilder>>(port)
        {

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
            switch (msg.header.id)
            {
            case Protocol_CreateAccount_Req:
            {
                std::cout << "[" << session << "]: " << "login_pkt Received" << std::endl;
                SamplePacketHandler::CreateAccount(session, msg);
            }
            break;
            case Protocol_Login_Req:
            {
                std::cout << "[" << session << "]: " << "login_pkt Received" << std::endl;
                SamplePacketHandler::LoginAccount(session, msg);

            }
            break;
            case Protocol_CheckCharacterNickname_Req:
            {
                std::cout << "[" << session << "]: " << "Select Character" << std::endl;
                SamplePacketHandler::SelectCharacterNickname(session, msg);
            }
            break;
            case Protocol_CreateCharacter_Req:
            {
                std::cout << "[" << session << "]: " << "Create Character" << std::endl;
                SamplePacketHandler::CreateCharacter(session, msg);
            }
            break;
            }
        }

    private:
    };

} // namespace net

#endif // !_NET_SERVER_H_
