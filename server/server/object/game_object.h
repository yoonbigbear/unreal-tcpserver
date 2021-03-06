#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "pch.h"
#include "network/client_session.h"

class GameObject
{
public:

    GameObject(uint32_t id)
        :obj_id_(id)
    ,field_id_(0) {}

    auto session() { return session_; }
    void session(SessionPtr session) { session_ = session; }
    bool IsPlayer() { return session_ ? true : false; }

    const uint32_t obj_id() { return obj_id_; }

    int32_t field_id() { return field_id_; }
    void field_id(int32_t id) { field_id_ = id; }

    TransformPtr transform() { return transform_; }
    void transform(TransformPtr transform) { transform_ = transform; }

    virtual void Update(float dt) abstract;

private:
    uint32_t obj_id_;
    int32_t field_id_;

    TransformPtr transform_;
    SessionPtr session_;
};

#endif // _GAME_OBJECT_H_