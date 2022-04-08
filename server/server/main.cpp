#include "pch.h"

#include "server/net_server.h"
#include "world/world_manager.h"

//덤프파일을 남겨준다.
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
    LOG_INFO("DEBUG_INFO");
    //덤프파일을 남겨준다. 미리 호출
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
        DEBUG_FMT_ERROR("Exception: %s", e.what());
    }

    return 0;
}
