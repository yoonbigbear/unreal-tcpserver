#include "pch.h"

#include "server/net_server.h"
#include "world/world_manager.h"

#include "navigation/navigation.h"
#include <filesystem>


//���������� �����ش�.
LONG __stdcall ExceptionCallBack(EXCEPTION_POINTERS* e)
{
    MINIDUMP_EXCEPTION_INFORMATION info = {0};
    info.ThreadId = ::GetCurrentThreadId();
    info.ExceptionPointers = e;
    info.ClientPointers = false;

    std::string stemp("dump.dmp");

    HANDLE file_handle = CreateFile(stemp.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file_handle, MiniDumpNormal, &info,
        NULL, NULL);

    return 0;
}

int main(int argc, char* argv[])
{
    LOG_INFO("DEBUG_INFO {}", 1);
    //���������� �����ش�. �̸� ȣ��
    SetUnhandledExceptionFilter(ExceptionCallBack);

    try
    {
        net::CustomServer server(60001);
        server.Start();

        /// ����ó�� ����� �Ǿ����� ����.

        std::thread t1([&]() 
            {
                while (true)
                {
                    server.Update(-1, true);
                }
            });

        auto start = std::chrono::high_resolution_clock::now();
        auto now = std::chrono::high_resolution_clock::now();
        while (true)
        {
            now = std::chrono::high_resolution_clock::now();
            auto interval = now - start;
            start = now;
            auto dt = interval.count() * 0.000'001;

            WorldManager::instance().Update(static_cast<float>(dt));
        }

        t1.join();

    }
    catch (std::exception e)
    {
        DEBUG_FMT_ERROR("Exception: %s", e.what());
    }

    return 0;
}
