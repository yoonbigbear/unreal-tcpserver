///////////////////////////////////////////
//기본 콘솔 디버그
///////////////////////////////////////////

//콘솔 출력에 색 넣어 구별하도록 추가
#define NONE    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#define RED     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#define GREEN   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define BLUE    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define YELLOW  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#define INTENSE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

#ifdef _DEBUG

//debug 콘솔로그
#define DEBUG_LOG_INFO(log)             std::printf("%s:%d:" log "\n", __FILE__, __LINE__);
#define DEBUG_LOG_WARNING(log)  YELLOW  std::printf("%s:%d:" log "\n", __FILE__, __LINE__); NONE
#define DEBUG_LOG_ERROR(log)    RED     std::printf("%s:%d:" log "\n", __FILE__, __LINE__); NONE

#define DEBUG_FMT_INFO(fmt, ...)            std::printf("%s:%d:" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);
#define DEBUG_FMT_WARNING(fmt, ...) YELLOW  std::printf("%s:%d:" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); NONE
#define DEBUG_FMT_ERROR(fmt, ...)   RED     std::printf("%s:%d:" fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); NONE

#else

#define DEBUG_LOG_INFO(log)
#define DEBUG_LOG_WARNING(log)
#define DEBUG_LOG_ERROR(log)

#define DEBUG_FMT_INFO(fmt, ...)
#define DEBUG_FMT_WARNING(fmt, ...) 
#define DEBUG_FMT_ERROR(fmt, ...)

#endif // DEBUG

//#pragma warning(C6067)

//assert check
#define  ASSERT_RETURN(expr, ret) if(expr){ DEBUG_LOG_ERROR(#expr); return ret;};
#define  ASSERT_ERROR(expr) if(expr) { DEBUG_LOG_ERROR(#expr);};



///////////////////////////////////////////
// spdlog 콘솔.
///////////////////////////////////////////

//spdlog 오픈소스로 변경
#define BEGIN_STOPWATCH(sw) spdlog::debug("Elapsed {}", sw);
#define END_STOPWATCH(sw) spdlog::debug("Elapsed {:.3}", sw);

#define LOG_INFO(fmt, ...)  spdlog::info(fmt, ##__VA_ARGS__);
#define LOG_WARNING(fmt, ...) spdlog::warn(fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) spdlog::error(fmt, ##__VA_ARGS__);
#define LOG_CRITICAL(fmt, ...) spdlog::critical(fmt, ##__VA_ARGS__);


// "Protocol_"을 뺀 패킷id
#define BUILD_PACKET(protocol, ...)   \
net::Packet pkt; \
pkt.id = Protocol_##protocol##;  \
flatbuffers::FlatBufferBuilder fbb(1024);   \
auto builder = account::Create##protocol##Direct(fbb, ##__VA_ARGS__);    \
fbb.Finish(builder);    \
pkt << fbb;

#define BUILD_SIMPLE_PACKET(protocol, ...)   \
net::Packet pkt; \
pkt.id = Protocol_##protocol##;  \
flatbuffers::FlatBufferBuilder fbb(1024);   \
auto builder = account::Create##protocol##(fbb, ##__VA_ARGS__);    \
fbb.Finish(builder);    \
pkt << fbb;

#define WAIT_UNTIL_BEGIN(protocol) \
while (true) \
 { \
  if (!c.Incoming().empty()) \
   { \
    auto msg = c.Incoming().pop_front(); \
    if (msg.msg.header.id == protocol) \
    {;

#define WAIT_UNTIL_END \
    }\
   }\
 };