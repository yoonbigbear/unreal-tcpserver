#include "NetworkManager.h"

#include <macro.h>
#include <account_generated.h>

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

    auto start_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now;
    while (true)
    {
        now = std::chrono::steady_clock::now();
        net::delta_time = std::chrono::duration_cast<std::chrono::microseconds>
            (now - start_time).count() * 0.000001f;
        start_time = std::chrono::steady_clock::now();


    }
}
