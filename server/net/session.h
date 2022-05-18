#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "packet_queue.h"
#include "message.h"
#include "packet.h"

using namespace boost;

namespace net
{
using SessionPtr = std::shared_ptr<Session>;

    class Session : public std::enable_shared_from_this<Session>
    {
    public:

        enum class owner
        {
            server,
            client
        };

        Session(owner parent, asio::io_context& io_context,
            asio::ip::tcp::socket&& socket, PacketQueue<PacketSession>& in)
            : io_context_(io_context), socket_(std::move(socket)), queue_for_send_(in)
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
                    WaitForRecv();
                }
            }
        }

        void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
        {
            if (owner_type_ == owner::client)
            {
                asio::async_connect(socket_, endpoints,
                    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                    {
                        //socket 연결 후 패킷 recv 시작
                        WaitForRecv();
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

        void Send(const Packet& msg)
        {
            asio::post(io_context_, [this, msg]()
                {
                    bool writing_message = !send_packet_queue_.empty();
                    send_packet_queue_.push_back(msg);
                    if (!writing_message)
                        WriteMessage();
                });
        }

    private:

        void WaitForRecv()
        {
            asio::async_read(socket_,
                asio::buffer(&recv_packet_queue_.front(),
                    recv_packet_queue_.front().size),
                [this](std::error_code ec, std::size_t length) {
                    if (!ec)
                    {
                        int i = 88;
                        int j = i - 55;

                    }
                    else
                    {
                        LOG_ERROR("[{}] Read Header Failure", id_);
                        Disconnect();
                    }
                });
        }
        void WriteMessage()
        {
            asio::async_write(socket_, 
                asio::buffer(&send_packet_queue_.front(), send_packet_queue_.front().size),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        send_packet_queue_.pop_front();
                    }
                    else
                    {
                        LOG_ERROR("[{}] Write Header Fail", id_);
                        Disconnect();
                    }
                }
            );
        }

    protected:
        asio::ip::tcp::socket socket_;
        asio::io_context& io_context_;

        std::deque<Packet> recv_packet_queue_;
        std::deque<Packet> send_packet_queue_;

        PacketQueue<PacketSession>& queue_for_send_;
        owner owner_type_ = owner::server;

        uint32_t id_ = 0;
    };
} // namespace net

#endif // _CONNECTION_H_