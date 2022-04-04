///////////////////////////////////////////
//�⺻ �ܼ� �����
///////////////////////////////////////////

//�ܼ� ��¿� �� �־� �����ϵ��� �߰�
#define NONE    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#define RED     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#define GREEN   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define BLUE    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define YELLOW  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
#define INTENSE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

#ifdef _DEBUG

//debug �ַܼα�
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

#pragma warning(C6067)

//assert check
#define  ASSERT_RETURN(expr, ret) if(expr){ DEBUG_LOG_ERROR(#expr); return ret;};
#define  ASSERT_ERROR(expr) if(expr) { DEBUG_LOG_ERROR(#expr);};



///////////////////////////////////////////
// spdlog �ܼ�.
///////////////////////////////////////////

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"
//spdlog ���¼ҽ��� ����
#define BEGIN_STOPWATCH(sw) spdlog::debug("Elapsed {}", sw);
#define END_STOPWATCH(sw) spdlog::debug("Elapsed {:.3}", sw);

#define LOG_INFO(log) SPDLOG_INFO(#log);
#define LOG_WARNING(log) SPDLOG_WARN(#log);
#define LOG_ERROR(log) SPDLOG_ERROR(#log);
#define LOG_CRITICAL(log) SPDLOG_CRITICAL(#log);