#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "packet_queue.h"
#include <concurrent_queue.h>

using namespace boost;

namespace net
{
    struct Packet
    {
        uint16_t id = 0;
        uint16_t size = 0;
        std::vector<uint8_t> body;
    };

    class Session;
    class PacketSession
    {
    public:
        PacketSession() = default;
        PacketSession(Packet&& packet, std::shared_ptr<Session> owner)
        {
            packet_ = packet;
            packet_owner_ = owner;
        }
        PacketSession(Packet& packet, std::shared_ptr<Session> owner)
        {
            packet_ = packet;
            packet_owner_ = owner;
        }
        Packet packet_;
        std::shared_ptr<Session> packet_owner_;
    };

    class Session : public std::enable_shared_from_this<Session>
    {
    public:

        enum class owner
        {
            server,
            client
        };
        Session() = delete;
        Session(owner parent, asio::io_context& io_context,
            asio::ip::tcp::socket&& socket, PacketQueue<PacketSession>& in)
            : io_context_(io_context), socket_(std::move(socket)), recv_packet_queue_(in)
        {
            owner_type_ = parent;
        }

        virtual ~Session() {}

        const uint64_t id() const { return id_; }

        int acct_id() { return acct_id_; }
        void acct_id(int id) { acct_id_ = id; }

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
            memset(&tempbuffer_, 0, sizeof(tempbuffer_));
            socket_.async_read_some(
                asio::buffer(tempbuffer_),
                [this](std::error_code ec, std::size_t length) {
                    if (!ec)
                    {
                        if (length > 0)
                        {
                            AddToRecvQueue();
                        }

                        WaitForRecv();

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

        void AddToRecvQueue()
        {
            PacketSession msg;

            int offset = 0;
            memcpy(&msg.packet_.id, tempbuffer_, sizeof(short));
            offset += sizeof(short);
            memcpy(&msg.packet_.size, tempbuffer_ + offset, offset);
            offset += sizeof(short);
            msg.packet_.body.resize(msg.packet_.size);
            memcpy(msg.packet_.body.data(), tempbuffer_ + offset, msg.packet_.size);
            msg.packet_owner_ = this->shared_from_this();
            recv_packet_queue_.push_back(msg);
        }
    protected:
        asio::ip::tcp::socket socket_;
        asio::io_context& io_context_;

        BYTE tempbuffer_[1024] = {};
        PacketQueue<PacketSession>& recv_packet_queue_;
        std::deque<Packet> send_packet_queue_;

        owner owner_type_ = owner::server;

        uint64_t id_ = 0;
        int acct_id_ = 0;
    };

  

    using PacketSessionPtr = std::shared_ptr<PacketSession>;
    using SessionPtr = std::shared_ptr<Session>;
} // namespace net

#endif // _CONNECTION_H_