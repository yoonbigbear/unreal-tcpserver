#include "character.h"

Character::Character(uint32_t obj_id, int char_class)
    :GameObject(obj_id)
    ,char_class_(static_cast<character_class>(char_class))
{
}

void Character::Update(float dt)
{
}
