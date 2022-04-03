#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <concurrent_queue.h>

#include <boost/asio.hpp>

#include <flatbuffers/flatbuffers.h>
#include <SamplePacket_generated.h>

#include "net_client.h"

#pragma comment(lib, "flatbuffers.lib")
#pragma comment(lib, "net.lib")

using boost::asio::ip::tcp;

enum {
    max_length = 1024
};

enum class protocol : unsigned short {
    kTextSend,
    kLogin,
    kCreateAccount,
    kCreateCharacter,
    kCheckCharacterNickname,

    kResultCode
};

enum class result_code : unsigned short {
    kCreateSuccess,
    kCreateFailed,

    kAleadyExist,
    kNotExist,

    kLoginSuccess,
    kLoginFailed,
};

class CustomClient : public net::ClientInterface<protocol>
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
                    case protocol::kTextSend:
                    {
                        auto text = flatbuffers::GetRoot<SamplePacket::textREQ>(msg.body.data());
                        std::cout << text->text()->c_str() << std::endl;
                    }
                    case protocol::kResultCode:
                    {
                        auto res = flatbuffers::GetRoot<SamplePacket::ResultCode>(msg.body.data());
                        auto code = static_cast<result_code>(res->rescode());
                        switch (code)
                        {
                        case result_code::kLoginSuccess:
                            std::cout << "login success" << std::endl;
                            break;
                        case result_code::kAleadyExist:
                            std::cout << "already exist" << std::endl;
                            break;
                        case result_code::kLoginFailed:
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
                    net::Message<protocol> msg;
                    msg.header.id = protocol::kCreateCharacter;
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

                //if (loggedin)
                //{
                //    std::getline(std::cin >> std::ws, chat);
                //    // 서버로 메세지 전송
                //    {
                //        net::Message<protocol> msg;
                //        msg.header.id = protocol::kTextSend;
                //        flatbuffers::FlatBufferBuilder builder(1024);
                //        auto text = builder.CreateString(chat);
                //        SamplePacket::textREQBuilder text_req_builder(builder);
                //        text_req_builder.add_text(text);
                //        auto ack = text_req_builder.Finish();
                //        builder.Finish(ack);
                //        msg << builder;
                //        c.Send(msg);
                //    }
                //}
                //else
                //{
                //   std::string id;
                //   std::string password;
                //   std::cout << "ID: ";
                //   std::cin >> id;
                //   std::cout << "PASSWORD: ";
                //   std::cin >> password;
                //   std::cout << "Sending... ";
                //   // 서버로 메세지 전송
                //   {
                //       net::Message<protocol> msg;
                //       msg.header.id = protocol::kCreateAccount;
                //       flatbuffers::FlatBufferBuilder builder(1024);
                //       auto login_id = builder.CreateString(id);
                //       auto login_password = builder.CreateString(password);
                //       SamplePacket::LoginReqBuilder login_req_builder(builder);
                //       login_req_builder.add_id(login_id);
                //       login_req_builder.add_pw(login_password);
                //       auto ack = login_req_builder.Finish();
                //       builder.Finish(ack);
                //       msg << builder;
                //       c.Send(msg);
                //   }
                //}
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