#include "character.h"

Character::Character(uint64_t char_id, int char_class)
    :char_class_(static_cast<character_class>(char_class))
    , char_id_(char_id)
{
}
