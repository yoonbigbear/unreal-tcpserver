#ifndef _CLIENT_INTERFACE_H_
#define _CLIENT_INTERFACE_H_

#include <pch.h>

namespace net {
    template<typename T>
    class ClientInterface
    {
    public:
        ClientInterface() {}

        virtual ~ClientInterface()
        {
            Disconnect();
        }


        bool Connect(const std::string& host, const uint16_t port)
        {
            try
            {
                asio::ip::tcp::resolver resolver(io_context_);
                asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                connection_ = std::make_unique<Session<T, flatbuffers::FlatBufferBuilder>>
                    (Session<T, flatbuffers::FlatBufferBuilder>::owner::client, io_context_,
                        asio::ip::tcp::socket(io_context_), message_in_);

                connection_->ConnectToServer(endpoints);

                thread_context_ = std::thread([this]()
                    {
                        io_context_.run();
                    });
            }
            catch (std::exception& e)
            {
                std::cerr << "client exception: " << e.what() << std::endl;
                return false;
            }

            return true;
        }

        void Disconnect()
        {
            if (IsConnected())
            {
                connection_->Disconnect();
            }

            io_context_.stop();

            if (thread_context_.joinable())
                thread_context_.join();

            connection_.release();
        }

        bool IsConnected()
        {
            if (connection_)
                return connection_->IsConnected();
            else
                return false;
        }

    public:
        void Send(const Message<T, flatbuffers::FlatBufferBuilder>& msg)
        {
            if (IsConnected())
                connection_->Send(msg);
        }

        PacketQueue<OwnedMessage<T, flatbuffers::FlatBufferBuilder>>& Incoming()
        {
            return message_in_;
        }

    protected:
        asio::io_context io_context_;
        std::thread thread_context_;
        std::unique_ptr<Session<T, flatbuffers::FlatBufferBuilder>> connection_;

    private:
        PacketQueue<OwnedMessage<T, flatbuffers::FlatBufferBuilder>> message_in_;
    };

    enum {
        max_length = 1024
    };
} // namespace net

#endif // _CLIENT_INTERFACE_H_