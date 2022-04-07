#ifndef _PCH_H_
#define _PCH_H_


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#pragma comment(lib, "net.lib")

#include <net_pch.h>
#include "server/server_enum.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

using flatbuffer = flatbuffers::FlatBufferBuilder;

#endif // !_PCH_H_