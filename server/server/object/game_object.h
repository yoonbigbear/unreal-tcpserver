#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "pch.h"

class GameObject
{
public:

    using Shared = std::shared_ptr<GameObject>;
    using Weak = std::weak_ptr<GameObject>;

    GameObject(uint64_t id)
        :obj_id_(id) {}

    const uint64_t obj_id() { return obj_id_; }
private:
    uint64_t obj_id_;
};

#endif // _GAME_OBJECT_H_