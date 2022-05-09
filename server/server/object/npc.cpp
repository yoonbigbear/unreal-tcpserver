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
    field_ = WorldManager::instance().field(field_id());;

    float p[3] = {};
    field_->navigation()->RandomPoint(p);

    transform_->position(p);
}

void Npc::Update(float dt)
{

    if (!move_)
    {
        float p[3] = {};
        field_->navigation()->RandomPoint(p);
        transform_->dest(p);
        move_ = true;
    }
    else
    {
        auto dir = transform_->dest() - transform_->position();
        b2Vec2 d(dir.x, dir.y);
        auto length = d.Normalize();

        if (length < 10)
        {
            LOG_INFO("arrived id:{} ", obj_id());
            move_ = false;
            return;
        }

        auto next = transform_->position() + (dt * 0.01 * d);
        transform_->position().Set(next.x, next.y, dir.z);
    }
}
