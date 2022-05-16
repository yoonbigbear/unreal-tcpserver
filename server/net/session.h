#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "packet_queue.h"
#include "message.h"

using namespace boost;

namespace net
{
    template<typename T, typename U>
    class Session : public std::enable_shared_from_this<Session<T, U>>
    {
    public:

        enum class owner
        {
            server,
            client
        };

        Session(owner parent, asio::io_context& io_context, asio::ip::tcp::socket&& socket, PacketQueue<OwnedMessage<T,U>>& in)
            : io_context_(io_context), socket_(std::move(socket)), message_in_(in)
        {
            owner_type_ = parent;
        }

        virtual ~Session() {}

        const uint32_t id() const { return id_; }

        virtual void ConnectToClient(uint64_t id = 0)
        {
            if (owner_type_ == owner::server)
            {
                if (socket_.is_open())
                {
                    id_ = id;
                    ReadHeader();
                }
            }
        }

        void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
        {
            if (owner_type_ == owner::client)
            {
                asio::async_connect(socket_, endpoints, [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                    {
                        if (!ec)
                        {
                            ReadHeader();
                            //������ ���� �������ش�.

                        }
                    });
            }
        }

        virtual void Disconnect()
        {
            if (IsConnected())
            {
                asio::post(io_context_, [this]() {socket_.close(); });
                LOG_INFO("Disconnected {}", id_);
            }
        }

        bool IsConnected() const
        {
            return socket_.is_open();
        }

        //void StartListening()
        //{

        //}

        void Send(const Message<T, U>& msg)
        {
            asio::post(io_context_, [this, msg]()
                {
                    bool writing_message = !message_out_.empty();
                    message_out_.push_back(msg);
                    if (!writing_message)
                        WriteHeader();
                });
        }

    private:
        void WriteHeader()
        {
            asio::async_write(socket_, asio::buffer(&message_out_.front().header, sizeof(MessageHeader<T>)),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        if (message_out_.front().body.size() > 0)
                        {
                            WriteBody();
                        }
                        else
                        {
                            message_out_.pop_front();

                            if (!message_out_.empty())
                            {
                                WriteHeader();
                            }
                        }
                    }
                    else
                    {
                        LOG_ERROR("[{}] Write Header Fail", id_);
                        Disconnect();
                    }
                });
        }

        void WriteBody()
        {
            asio::async_write(socket_, asio::buffer(message_out_.front().body.data(), message_out_.front().body.size()),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        message_out_.pop_front();
                        if (!message_out_.empty())
                        {
                            WriteHeader();
                        }
                    }
                    else
                    {
                        LOG_ERROR("[{}] Write Body Fail", id_);
                        Disconnect();
                    }
                });
        }

        void WaitForRecv()
        {
            socket_.async_receive(asio::buffer(packet_queue_.front(), packet_queue_.front().header.bodysize),
                )
        }
        void ReadHeader()
        {
            asio::async_read(socket_, asio::buffer(&temporary_in_.header, sizeof(MessageHeader<T>)),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        if (temporary_in_.header.size > 0)
                        {
                            temporary_in_.body.resize(temporary_in_.header.size);
                            ReadBody();
                        }
                        else
                        {
                            AddToIncomingMessageQueue();
                        }
                    }
                    else
                    {
                        LOG_ERROR("[{}] Read Header Failure", id_);
                        Disconnect();
                    }
                });
        }

        void ReadBody()
        {
            asio::async_read(socket_, asio::buffer(temporary_in_.body.data(), temporary_in_.body.size()),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        AddToIncomingMessageQueue();
                    }
                    else
                    {
                        LOG_ERROR("[{}] Read Body Failure", id_);
                        Disconnect();
                    }
                });
        }

        void AddToIncomingMessageQueue()
        {
            if (owner_type_ == owner::server)
                message_in_.push_back({ this->shared_from_this(), temporary_in_ });
            else
                message_in_.push_back({ nullptr, temporary_in_ });

            ReadHeader();
        }

    protected:
        asio::ip::tcp::socket socket_;
        asio::io_context& io_context_;
        PacketQueue<Message<T,U>> message_out_;
        PacketQueue<OwnedMessage<T,U>>& message_in_;

        PacketQueue<Packet> packet_queue_;

        Message<T, U> temporary_in_;
        owner owner_type_ = owner::server;

        uint64_t id_ = 0;
    };
} // namespace net

#endif // _CONNECTION_H_