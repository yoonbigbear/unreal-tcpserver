#pragma once
#include <pch.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#pragma comment(lib, "net.lib")

#include <SamplePacket_generated.h>
#include <protocol_generated.h>
#include <result_code_generated.h>

using namespace boost;
using namespace net;

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

            connection_ = std::make_unique<Session<Protocol, flatbuffers::FlatBufferBuilder>>
                (Session<Protocol, flatbuffers::FlatBufferBuilder>::owner::client, io_context_,
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
    void Send(const Message<Protocol, flatbuffers::FlatBufferBuilder>& msg)
    {
        if (IsConnected())
            connection_->Send(msg);
    }

    PacketQueue<OwnedMessage<Protocol, flatbuffers::FlatBufferBuilder>>& Incoming()
    {
        return message_in_;
    }

protected:
    asio::io_context io_context_;
    std::thread thread_context_;
    std::unique_ptr<Session<Protocol, flatbuffers::FlatBufferBuilder>> connection_;

private:
    PacketQueue<OwnedMessage<Protocol, flatbuffers::FlatBufferBuilder>> message_in_;
};

enum {
    max_length = 1024
};

class CustomClient : public ClientInterface
{
public:

};

bool loggedin = false;

static HANDLE Screen[2];

int main(int argc, char* argv[])
{
    try
    {
        CustomClient c;
        c.Connect("127.0.0.1", 60000);

        bool quit = false;

        std::string chat;

        std::thread msg_recv([&c]() {

            while (1)
            {
                if (!c.Incoming().empty())
                {
                    auto msg = c.Incoming().pop_front().msg;

                    //msg 수신
                    switch (msg.header.id)
                    {
                    case Protocol_TextSend:
                    {
                        auto text = flatbuffers::GetRoot<SamplePacket::textREQ>(msg.body.data());
                        std::cout << text->text()->c_str() << std::endl;
                    }
                    case Protocol_ResultCode:
                    {
                        auto res = flatbuffers::GetRoot<SamplePacket::Result>(msg.body.data());
                        auto code = static_cast<ResultCode>(res->rescode());
                        switch (code)
                        {
                        case ResultCode_LoginSuccess:
                            std::cout << "login success" << std::endl;
                            break;
                        case ResultCode_AleadyExist:
                            std::cout << "already exist" << std::endl;
                            break;
                        case ResultCode_LoginFailed:
                            std::cout << "login failed" << std::endl;
                            break;
                        }
                    }
                    break;

                    }
                }
            }});

        while (!quit)
        {
            if (c.IsConnected())
            {
                std::string id;
                int password;

                std::cout << "Nickname: ";
                std::cin >> id;

                std::cout << "Class: ";
                std::cin >> password;

                std::cout << "Sending... ";
                // 서버로 메세지 전송
                {
                    net::Message<Protocol, flatbuffers::FlatBufferBuilder> msg;
                    msg.header.id = Protocol_CreateCharacter;
                    flatbuffers::FlatBufferBuilder builder(1024);
                    auto login_id = builder.CreateString(id);
                    SamplePacket::CreateCharacterReqBuilder create_character_builder(builder);
                    create_character_builder.add_nickname(login_id);
                    create_character_builder.add_class_(password);
                    auto ack = create_character_builder.Finish();
                    builder.Finish(ack);
                    msg << builder;
                    c.Send(msg);
                }
            }
            else
            {
                std::cout << "Server Down" << std::endl;
                quit = true;
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}