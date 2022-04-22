#include "world_manager.h"

#include "zone.h"

WorldManager::WorldManager()
{
    auto newfield = std::make_shared<Field>();
    fields_.emplace(0, newfield);
}

WorldManager::~WorldManager()
{
}

void WorldManager::EnterField(int field_id, GameObject::Shared obj)
{
    auto field = fields_.at(field_id);
    if (field)
    {
        field->Enter(obj);
    }
}

void WorldManager::LeaveField()
{
}
