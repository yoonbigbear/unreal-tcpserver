#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <macro.h>
#include <client_interface.h>
#include <account_generated.h>  
#include <protocol_generated.h>
#include <result_code_generated.h>


class CustomClient : public net::ClientInterface
{
public:

};

bool loggedin = false;

static HANDLE Screen[2];

void PacketTest(CustomClient& c);

int main(int argc, char* argv[])
{
    try
    {
        CustomClient c;
        c.Connect("127.0.0.1", 11000);
        if (c.IsConnected())
        {
            std::thread t([&]() {
                while (true)
                {

                }
                });

            PacketTest(c);
        }
        else
        {
            std::cout << "Server Down" << std::endl;
            return 0;
        }


    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}