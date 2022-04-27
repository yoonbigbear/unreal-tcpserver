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

void WorldManager::Start()
{
    CreateSampleAI(0, 10);
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

void WorldManager::CreateSampleAI(int field_id, int count)
{
    for (int i = 0; i < count; ++i)
    {
        GameObject::Shared game_object = std::make_shared<GameObject>(ai_id_count_++);
        game_object->CreateTransform();

        auto field = fields_.at(field_id);
        if (field)
        {
            field->Enter(game_object);
        }
    }

}

void WorldManager::Update()
{
    for (auto e : fields_)
    {
        e.second->Update();
    }
}
