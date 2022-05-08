#include "npc.h"

#include "world/world_manager.h"
#include "components/transform.h"
#include "navigation/navigation.h"

Npc::Npc(uint32_t obj_id)
    :GameObject(obj_id)
{
}

void Npc::start()
{
    transform_ = std::make_shared<Transform>();
}

void Npc::Update()
{
    auto field = WorldManager::instance().field(field_id());

}
