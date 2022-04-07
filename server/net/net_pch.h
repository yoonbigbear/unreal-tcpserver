#ifndef _NET_PCH_H_
#define _NET_PCH_H_


#pragma warning(disable:4996) //CRT_NO_WARNING

// 여기에 미리 컴파일하려는 헤더 추가
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <deque>
#include <mutex>

#include <boost/asio.hpp>
#include <flatbuffers/flatbuffers.h>
#include <csv.hpp>
#include <json.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

#include "macro.h"
#include "session.h"
#include "message.h"
#include "packet_queue.h"



using json = nlohmann::json;


#endif //_NET_PCH_H_