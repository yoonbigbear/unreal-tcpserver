#ifndef _PCH_H_
#define _PCH_H_

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

#pragma warning(disable:4996)
#pragma comment(lib, "flatbuffers.lib")
#pragma comment(lib, "net.lib")

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <boost/asio.hpp>
#include <SamplePacket_generated.h>

#include "macro.h"
#include "enums.h"

#include "header_only/csv.hpp"
#include "header_only/json.hpp"

#include "network/samplepacket/samplepacket_helper.h"
#include "database/DB.h"
#include "behaiviour_tree/btree.h"
#include "net_server.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")



using json = nlohmann::json;
using boost::asio::ip::tcp;

#endif