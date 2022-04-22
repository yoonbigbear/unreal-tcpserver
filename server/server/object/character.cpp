#include "character.h"

Character::Character(unsigned long long char_id, int char_class)
    :GameObject(char_id)
    ,char_class_(static_cast<character_class>(char_class))
{
    transform_ = std::make_shared<Transform>();
}
