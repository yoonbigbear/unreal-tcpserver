#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <memory>

#include <boost/asio.hpp>

#include "message.h"
#include "packet_queue.h"

using namespace boost;

namespace net
{
    template<typename T>
    class Session : public std::enable_shared_from_this<Session<T>>
    {
    public:
        enum class owner
        {
            server,
            client
        };

        Session(owner parent, asio::io_context& io_context, asio::ip::tcp::socket&& socket, PacketQueue<OwnedMessage<T>>& in)
            : io_context_(io_context), socket_(std::move(socket)), message_in_(in)
        {
            owner_type_ = parent;
        }

        virtual ~Session() {}

        uint32_t GetId() const { return id_; }

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
                            //세션을 만들어서 관리해준다.

                        }
                    });
            }
        }

        void Disconnect()
        {
            if (IsConnected())
                asio::post(io_context_, [this]() {socket_.close(); });
        }

        bool IsConnected() const
        {
            return socket_.is_open();
        }

        //void StartListening()
        //{

        //}

        void Send(const Message<T>& msg)
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
                        //std::cout << "[" << id_ << "] Write Header Fail." << std::endl;
                        socket_.close();
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
                        //std::cout << "[" << id_ << "] Write Body Fail" << std::endl;
                        socket_.close();
                    }
                });
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
                       // std::cout << "[" << id_ << "] Read Header Fail" << std::endl;
                        socket_.close();
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
                        //std::cout << "[" << id_ << "] Read Body Fail" << std::endl;
                        socket_.close();
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
        PacketQueue<Message<T>> message_out_;
        PacketQueue<OwnedMessage<T>>& message_in_;
        Message<T> temporary_in_;
        owner owner_type_ = owner::server;

        uint64_t id_ = 0;
    };
} // namespace net

#endif // _CONNECTION_H_