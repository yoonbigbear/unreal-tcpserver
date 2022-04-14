#ifndef _DB_H_
#define _DB_H_

#include "pch.h"
#include "nanodbc/nanodbc.h"

class DB
{

public:
    static void add_data(int64_t id);
    static void add_chat(std::string_view chat);

    static int select_account(std::string_view id, std::string_view password, nanodbc::result& res);
    static int create_account(std::string_view id, std::string_view password);

    static int select_character_nickname(std::string_view nickname);
    static int create_character(uint64_t char_id, std::string_view nickname, int class_id);
};

#endif //DB_H_