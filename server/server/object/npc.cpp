#include "npc.h"

#include "world/world_manager.h"
#include "components/transform.h"
#include "navigation/navigation.h"

#include <protocol_generated.h>
#include <world_generated.h>

Npc::Npc(uint32_t obj_id)
    :GameObject(obj_id)
{
}

void Npc::start()
{
    transform(std::make_shared<Transform>());
    field_ = WorldManager::instance().field(field_id());;

    float p[3] = {};
    field_->navigation()->RandomPoint(p);

    transform()->position(p);
}

void Npc::Update(float dt)
{

    if (!move_)
    {
        float p[3] = {};
        field_->navigation()->RandomPoint(p);
        transform()->dest(p);
        move_ = true;

        auto dir = transform()->dest() - transform()->position();
        b2Vec2 d(dir.x, dir.y);
        auto length = d.Normalize();
        auto vec2 = Vec2(d.x, d.y);

        Packet pkt;
        pkt.id = Protocol_MoveStartSync;
        flatbuffer fbb(1024);

        auto builder = world::CreateMoveStartSync(fbb, obj_id(), &vec2, 0.01f);
        fbb.Finish(builder);
        pkt.size = fbb.GetSize();
        pkt.body.resize(pkt.size);
        std::memcpy(pkt.body.data(), fbb.GetBufferPointer(), pkt.size);
        field_->Broadcast(pkt);
    }
    else
    {
        auto dir = transform()->dest() - transform()->position();
        b2Vec2 d(dir.x, dir.y);
        auto length = d.Normalize();

        if (length < 1)
        {
            LOG_INFO("obj:{} pos: {} {} {}", obj_id(),
                transform()->position().x,
                transform()->position().y,
                transform()->position().z);

            move_ = false;

            auto vec3 = Vec3(transform()->position().x, transform()->position().y, transform()->position().z);

            Packet pkt;
            pkt.id = Protocol_MoveStopSync;
            flatbuffer fbb(1024);
            auto builder = world::CreateMoveStopSync(fbb, obj_id(), &vec3);
            fbb.Finish(builder);
            pkt.size = fbb.GetSize();
            pkt.body.resize(pkt.size);
            std::memcpy(pkt.body.data(), fbb.GetBufferPointer(), pkt.size);
            field_->Broadcast(pkt);
            return;
        }

        auto next = transform()->position() + (dt * 0.01f * d);
        transform()->position().Set(next.x, next.y, transform()->position().z);
    }
}
