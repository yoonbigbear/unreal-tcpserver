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
        //������ ����. ���� ���� ��ġ�� �̵��Ѵ�.

        //������ ����
        float p[3] = {};
        field_->navigation()->RandomPoint(p);
        transform()->dest(p);
        move_ = true;

        //������ ���� (vec2)
        auto dir = transform()->dest() - transform()->position();
        b2Vec2 d(dir.x, dir.z);
        auto length = d.Normalize();
        auto vec2 = Vec2(d.x, d.y);

        //��������
        net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
        pkt.header.id = Protocol_MoveStartSync;
        flatbuffer fbb(1024);

        auto builder = world::CreateMoveStartSync(fbb, obj_id(), &vec2, spd_);
        fbb.Finish(builder);
        pkt << fbb;
        field_->Broadcast(pkt);
    }
    else
    {
        // �������� ���� ���´� �̵��Ѵ�.
        // 
        // �̵� ����
        auto dir = transform()->dest() - transform()->position();
        b2Vec2 d(dir.x, dir.z);
        auto length = d.Normalize();

        //�������� ���
        if (length < 1)
        {
            LOG_INFO("obj:{} pos: {} {} {}", obj_id(),
                transform()->position().x,
                transform()->position().y,
                transform()->position().z);

            move_ = false;

            auto vec3 = Vec3(transform()->position().x, transform()->position().y, transform()->position().z);

            net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
            pkt.header.id = Protocol_MoveStopSync;
            flatbuffer fbb(1024);
            auto builder = world::CreateMoveStopSync(fbb, obj_id(), &vec3);
            fbb.Finish(builder);
            pkt << fbb;
            field_->Broadcast(pkt);
            return;
        }

        // ��ġ �̵�
        b2Vec2 pos = b2Vec2(transform()->position().x, transform()->position().z);
        auto next = pos + (dt * spd_ * d);
        transform()->position().Set(next.x, transform()->position().y, next.y);

        //LOG_WARNING("obj:{} moving: {} {} {}", obj_id(),
        //    transform()->position().x,
        //    transform()->position().y,
        //    transform()->position().z);
    }
}
