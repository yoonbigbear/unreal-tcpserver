#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "pch.h"

#include "network/client_session.h"
#include "character.h"

#include "components/transform.h"
#include "components/ai.h"

class GameObject
{
public:

    using Shared = std::shared_ptr<GameObject>;
    using Weak = std::weak_ptr<GameObject>;

    GameObject(uint64_t id)
        :obj_id_(id) {}

    auto session() { return session_; }
    void session(SessionPtr session) { session_ = session; }

    const uint64_t obj_id() { return obj_id_; }
    auto character() { return character_; }
    auto transform() { return transform_; }
    auto ai() { return ai_; }

public:
    void CreateCharacter(int char_class);
    void CreateTransform();
    void CreateAI();

private:
    uint64_t obj_id_;

    SessionPtr session_;
    CharacterPtr character_;
    TransformPtr transform_;
    AIPtr ai_;
};

#endif // _GAME_OBJECT_H_