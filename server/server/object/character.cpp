#include "character.h"
#include "world/world_manager.h"
#include "components/transform.h"

Character::Character(uint32_t obj_id, int char_class)
    :GameObject(obj_id)
    ,char_class_(static_cast<character_class>(char_class))
{
}

void Character::start()
{
    transform(std::make_shared<Transform>());
    field_ = WorldManager::instance().field(field_id());;
}

void Character::Update(float dt)
{
}
