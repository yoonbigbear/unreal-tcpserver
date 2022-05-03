#include "npc.h"

#include "components/transform.h"

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
    if (!is_moving)
    {
        is_moving = transform_->MoveTo();
    }
    else
    {
        is_moving = false;
        transform_->dest(transform_->position() + b2Vec3(5, 5, 5));
    }
}
