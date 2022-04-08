#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include "pch.h"

#include <protocol_generated.h>

#include "network/client_session.h"
#include "network/samplepacket/samplepacket_handler.h"

using namespace boost;

namespace net
{
    class ServerInterface
    {
    public:
        ServerInterface(uint16_t port)
            :acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {

        }

        virtual ~ServerInterface()
        {
            Stop();
        }

        bool Start()
        {
            try
            {
                WaitForClientConnection();
                thread_context_ = std::thread([this]() {io_context_.run(); });
            }
            catch (std::exception& e)
            {
                std::cerr << "[SERVER] Exception: " << e.what() << std::endl;
                return false;
            }
            return true;
        }

        void Stop()
        {
            io_context_.stop();
            if (thread_context_.joinable())
                thread_context_.join();
            std::cout << "[SERVER] Stopped" << std::endl;
        }

        void WaitForClientConnection()
        {
            acceptor_.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket)
                {
                    if (!ec)
                    {
                        std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;
                        auto newconn =
                            std::make_shared<ClientSession<Protocol,flatbuffer>>(ClientSession<Protocol, flatbuffer>::owner::server,
                                io_context_, std::move(socket), message_in_);

                        if (OnClientConnect(newconn))
                        {
                            sessions_.push_back(std::move(newconn));
                            sessions_.back()->ConnectToClient(id_counter_++);

                            std::cout << "[" << sessions_.back() << "] Connection Approved" << std::endl;
                        }
                        else
                        {
                            std::cout << "[-----] Connection Denied" << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "[SERVER] New Connection Error: " << ec.message() << std::endl;
                    }
                    WaitForClientConnection();
                });
        }

        void MessageClient(ClientSession<Protocol, flatbuffer>::Shared client, const Message<Protocol, flatbuffer>& msg)
        {
            if (client && client->IsConnected())
            {
                client->Send(msg);
            }
            else
            {
                OnClientDisconnect(client);
                client.reset();
                sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), client), sessions_.end());
            }
        }

        void MessageAllClients(const Message<Protocol, flatbuffer> msg, ClientSession<Protocol, flatbuffer>::Shared ignore_client = nullptr)
        {
            bool invalid_client_exists = false;

            for (auto& client : sessions_)
            {
                if (client && client->IsConnected())
                {
                    if (client != ignore_client)
                        client->Send(msg);
                }
                else
                {
                    OnClientDisconnect(client);
                    client.reset();

                    invalid_client_exists = true;
                }
            }

            if (invalid_client_exists)
                sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), nullptr), sessions_.end());
        }

        void Update(size_t max_messages = -1, bool wait = false)
        {
            if (wait)
                message_in_.wait();

            size_t message_count = 0;
            while (message_count < max_messages && !message_in_.empty())
            {
                auto msg = message_in_.pop_front();
                auto clientsession = std::static_pointer_cast<ClientSession<Protocol, flatbuffer>>(msg.remote);
                OnMessage(clientsession, msg.msg);
                message_count++;
            }
        }

    protected:
        virtual bool OnClientConnect(ClientSession<Protocol, flatbuffer>::Shared client)
        {
            return false;
        }

        virtual void OnClientDisconnect(ClientSession<Protocol, flatbuffer>::Shared client)
        {

        }

        virtual void OnMessage(ClientSession<Protocol, flatbuffer>::Shared client, Message<Protocol, flatbuffer>& msg)
        {

        }

    protected:
        PacketQueue<OwnedMessage<Protocol, flatbuffer>> message_in_;
        std::deque<ClientSession<Protocol, flatbuffer>::Shared> sessions_;
        asio::io_context io_context_;
        std::thread thread_context_;
        asio::ip::tcp::acceptor acceptor_;

        uint32_t id_counter_ = 10000;
    };


    class CustomServer : public ServerInterface
    {
    public:
        CustomServer(uint16_t port) : ServerInterface(port)
        {

        }

    protected:
        virtual bool OnClientConnect(ClientSession<Protocol, flatbuffer>::Shared client)
        {
            std::cout << "클라 세션 붙었음" << std::endl;

            return true;
        }

        virtual void OnClientDisconnect(ClientSession<Protocol, flatbuffer>::Shared client)
        {
            std::cout << "Removing client [" << client << "]" << std::endl;
        }

        virtual void OnMessage(ClientSession<Protocol, flatbuffer>::Shared session, Message<Protocol, flatbuffer>& msg)
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
