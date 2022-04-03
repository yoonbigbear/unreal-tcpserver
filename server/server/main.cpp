#include "pch.h"

#include "net_server.h"

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
    SetUnhandledExceptionFilter(ExceptionCallBack);
    try
    {
        net::CustomServer server(60000);
        server.Start();

        while (true)
        {
            server.Update(-1, true);
        }

    }
    catch (std::exception e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
