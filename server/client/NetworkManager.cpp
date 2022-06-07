#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>
#include <world_generated.h>
#include <common_generated.h>

struct GameObject
{
    Vec3 v;
    float spd;
    Vec2 d;
    bool move = false;
};

inline Vec2 operator * (float s, const Vec2& a)
{
    return Vec2(s * a.x(), s * a.y());
}

inline Vec3 operator +(Vec3 v, Vec2&& pos2d)
{
    return Vec3(v.x() + pos2d.x(), v.y() + pos2d.y(), v.z());
}

void PacketTest(CustomClient& c)
{
    //CreateAccount Pkt test
    /*{
        BUILD_PACKET(CreateAccountReq, "sampleId", "samplePassword");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateAccountReq);

        auto body = flatbuffers::GetRoot<account::CreateAccountAck>(msg.msg.body.data());
        std::cout << "pkt received:" << body->result() << std::endl;

        break;
        WAIT_UNTIL_END;
    }*/

    //login account
    {
        BUILD_PACKET(LoginReq, "sampleId", "samplePassword");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_LoginAck);

        auto body = flatbuffers::GetRoot<account::LoginAck>(msg.msg.body.data());
        if (body->result() == ResultCode_LoginSuccess)
        {
            if (body->characters()->size() > 0)
            {
                LOG_INFO("로그인 성공.");
                for (int i = 0; i < body->characters()->size(); ++i)
                {
                    auto info = body->characters()->Get(i);
                    auto nickname = info->nickname()->str();
                    auto job = info->class_();
                    auto id = info->char_id();
                    LOG_INFO("character id:{} nickname:{} job:{}", id, nickname, job);
                }
            }
            else
            {
                LOG_INFO("로그인 성공. 가진 캐릭터 없음");
            }
        }
        else
        {
            LOG_INFO("로그인 실패. 없는 아이디 비밀번호");
        }


        break;
        WAIT_UNTIL_END;
    }

    //create character
    /*{
        BUILD_PACKET(CreateCharacterReq, "sampleCharacter", 1);
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CreateCharacterAck);

        auto body = flatbuffers::GetRoot<account::CreateCharacterAck>(msg.msg.body.data());
        DEBUG_LOG_INFO("result %d", body->result());

        break;
        WAIT_UNTIL_END;
    }*/

    //Check character nickname exists
    /*{
        BUILD_PACKET(CheckCharacterNicknameReq, "sampleCharacter");
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_CheckCharacterNicknameAck);

        auto body = flatbuffers::GetRoot<account::CheckCharacterNicknameAck>(msg.msg.body.data());
        if (body->result() == ResultCode::ResultCode_AleadyExist)
        {
            LOG_INFO("이미 있는 닉네임");
        }
        else
        {
            LOG_INFO("사용 가능");
        }

        break;
        WAIT_UNTIL_END;
    }*/

    // select character
    {
        BUILD_SIMPLE_PACKET(SelectCharacterReq, 484280193545015296);
        c.Send(pkt);

        WAIT_UNTIL_BEGIN(Protocol::Protocol_SelectCharacterAck);

        auto body = flatbuffers::GetRoot<account::SelectCharacterAck>(msg.msg.body.data());
        if (body->result() == ResultCode::ResultCode_EnterGameSuccess)
        {
            LOG_INFO("게임 입장 패킷 수신");
            auto pos = body->position();
            LOG_INFO("좌표 {}, {}, {}", pos->x(), pos->y(), pos->z());
        }
        else
        {
            LOG_INFO("게임 입장 실패");
        }

        break;
        WAIT_UNTIL_END;
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();

    std::unordered_map<uint32_t, GameObject> objs;
    while (true)
    {
        now = std::chrono::high_resolution_clock::now();
        auto interval = now - start;
        start = now;
        auto dt = interval.count() * 0.000'001;


        if (!c.Incoming().empty())
        {
            auto msg = c.Incoming().pop_front();

            switch (msg.msg.header.id)
            {
            case Protocol::Protocol_EnterFieldSync:
            {
                auto body = flatbuffers::GetRoot<world::EnterFieldSync>(msg.msg.body.data());
                if (body)
                {
                    auto id_list = body->obj_id();
                    auto pos_list = body->pos();

                    auto count = id_list->size();

                    int id;
                    GameObject obj;
                    Vec3 pos;
                    for (int i = 0; i < count; ++i)
                    {
                        id = id_list->Get(i);
                        pos = *pos_list->Get(i);

                        LOG_INFO("[입장] obj:{} x: {} y: {} z: {}", id, pos.x(), pos.y(), pos.z());
                        obj.v = pos;
                        objs[id] = std::move(obj);

                    }
                }
            }
            break;

            case Protocol::Protocol_MoveStartSync:
            {
                auto body = flatbuffers::GetRoot<world::MoveStartSync>(msg.msg.body.data());
                if (body)
                {
                    auto id = body->obj_id();
                    Vec2 dir = *body->dir();
                    float spd = body->speed();
                    //LOG_INFO("obj:{} dir: {} {}", id, dir.x(), dir.y());

                    objs[id].d = dir;
                    objs[id].spd = spd;
                    objs[id].move = true;
                }
            }
            break;

            case Protocol::Protocol_MoveStopSync:
            {
                auto body = flatbuffers::GetRoot<world::MoveStopSync>(msg.msg.body.data());
                if (body)
                {
                    auto id = body->obj_id();
                    Vec3 pos = *body->pos();
                    LOG_INFO("obj:{} pos: {} {} {}", id, objs[id].v.x(), objs[id].v.y(), objs[id].v.z());
                    objs[id].d = Vec2(0, 0);
                    objs[id].move = false;
                }
            }
            break;
            }
        }

        for (auto& e : objs)
        {
            if (e.second.move)
            {
                auto next = e.second.v + (static_cast<float>(dt) * 0.01 * e.second.d);
                e.second.v = next;
            }
        }
    }
}
