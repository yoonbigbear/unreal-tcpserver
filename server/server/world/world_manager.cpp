#include "world_manager.h"

#include "zone.h"

#include "object/npc.h"
#include "components/transform.h"

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
    CreateSampleAI(0, 1);
}

void WorldManager::EnterField(int field_id, GameObjectPtr obj)
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
        NpcPtr npc = std::make_shared<Npc>(ai_id_count_++);
        npc->start();

        auto field = fields_.at(field_id);
        if (field)
        {
            field->Enter(npc);
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
