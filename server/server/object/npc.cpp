#include "npc.h"

#include "components/transform.h"

Npc::Npc(uint32_t obj_id)
    :GameObject(obj_id)
{
}

void Npc::start()
{
    transform_ = std::make_shared<Transform>();
    transform_->dest() += b2Vec2(5000, 5000);
}

void Npc::Update()
{
    if (!transform_->MoveTo())
    {
        transform_->dest(transform_->position() + b2Vec2(5000, 5000));
    }
}
