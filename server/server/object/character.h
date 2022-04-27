#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "server/server_enum.h"

class Character
{
public:
    Character() = delete;
    Character(int char_class);

    int map_id() { return map_id_; }
    void map_id(int map_id) { map_id_ = map_id; }

private:

    int map_id_ = 0;
    character_class char_class_;
};

using CharacterPtr = std::shared_ptr<Character>;

#endif // !_CHARACTER_H_


