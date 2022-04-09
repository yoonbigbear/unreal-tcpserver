#ifndef _SERVER_INTERFACE_H_
#define _SERVER_INTERFACE_H_

#include "pch.h"

namespace net {



    template<typename T, typename U>
    class ServerInterface
    {
    public:
        using flatbuffer = flatbuffers::FlatBufferBuilder;

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
                            std::make_shared<U>(U::owner::server,
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

        void MessageClient(std::shared_ptr<U> client, const Message<T, flatbuffer>& msg)
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

        void MessageAllClients(const Message<T, flatbuffer> msg, std::shared_ptr<U> ignore_client = nullptr)
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
                auto clientsession = std::static_pointer_cast<U>(msg.remote);
                OnMessage(clientsession, msg.msg);
                message_count++;
            }
        }

    protected:
        virtual bool OnClientConnect(std::shared_ptr<U> client)
        {
            return false;
        }

        virtual void OnClientDisconnect(std::shared_ptr<U> client)
        {

        }

        virtual void OnMessage(std::shared_ptr<U> client, Message<T, flatbuffer>& msg)
        {

        }

    protected:
        PacketQueue<OwnedMessage<T, flatbuffer>> message_in_;
        std::deque<std::shared_ptr<U>> sessions_;
        asio::io_context io_context_;
        std::thread thread_context_;
        asio::ip::tcp::acceptor acceptor_;

        uint32_t id_counter_ = 10000;
    };

} // namespace net

#endif // !_SERVER_INTERFACE_H_
