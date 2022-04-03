#ifndef _NET_SERVER_H_
#define _NET_SERVER_H_

#include <deque>

#include <boost/asio.hpp>

#include "message.h"
#include "network/client_session.h"
#include "packet_queue.h"

#include "network/protocol.h"
#include "network/samplepacket/samplepacket_handler.h"

using namespace boost;

namespace net
{
    template<typename T>
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
                        ClientSession::Shared newconn = std::make_shared<ClientSession>(ClientSession::owner::server,
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

        void MessageClient(ClientSession::Shared client, const Message<T>& msg)
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

        void MessageAllClients(const Message<T> msg, ClientSession::Shared ignore_client = nullptr)
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
                auto clientsession = std::static_pointer_cast<ClientSession>(msg.remote);
                OnMessage(clientsession, msg.msg);
                message_count++;
            }
        }

    protected:
        virtual bool OnClientConnect(ClientSession::Shared client)
        {
            return false;
        }

        virtual void OnClientDisconnect(ClientSession::Shared client)
        {

        }

        virtual void OnMessage(ClientSession::Shared client, Message<T>& msg)
        {

        }

    protected:
        PacketQueue<OwnedMessage<T>> message_in_;
        std::deque<ClientSession::Shared> sessions_;
        asio::io_context io_context_;
        std::thread thread_context_;
        asio::ip::tcp::acceptor acceptor_;

        uint32_t id_counter_ = 10000;
    };


    class CustomServer : public ServerInterface<protocol>
    {
    public:
        CustomServer(uint16_t port) : ServerInterface<protocol>(port)
        {

        }

    protected:
        virtual bool OnClientConnect(ClientSession::Shared client)
        {
            std::cout << "클라 세션 붙었음" << std::endl;

            return true;
        }

        virtual void OnClientDisconnect(ClientSession::Shared client)
        {
            std::cout << "Removing client [" << client << "]" << std::endl;
        }

        virtual void OnMessage(ClientSession::Shared session, Message<protocol>& msg)
        {
            switch (msg.header.id)
            {
            case protocol::kCreateAccount:
            {
                std::cout << "[" << session << "]: " << "login_pkt Received" << std::endl;
                SamplePacketHandler::CreateAccount(session, msg);
            }
            break;
            case protocol::kLogin:
            {
                std::cout << "[" << session << "]: " << "login_pkt Received" << std::endl;
                SamplePacketHandler::LoginAccount(session, msg);

            }
            break;
            case protocol::kTextSend:
            {
                std::cout << "[" << session << "]: " << "Text Received" << std::endl;
                SamplePacketHandler::SendText(session, msg);
                MessageAllClients(msg, session);
            }
            break;
            case protocol::kCheckCharacterNickname:
            {
                std::cout << "[" << session << "]: " << "Select Character" << std::endl;
                SamplePacketHandler::SelectCharacterNickname(session, msg);
            }
            break;
            case protocol::kCreateCharacter:
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
