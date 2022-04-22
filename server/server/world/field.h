#ifndef _FIELD_H_
#define _FIELD_H_

#include "pch.h"
#include "object/game_object.h"


class GameObject;

class Field
{
public:

    using Shared = std::shared_ptr<Field>;
    using Weak = std::weak_ptr<Field>;

    void Enter(GameObject::Shared obj);
    void Leave();

private:

    std::unordered_map<uint64_t, GameObject::Shared> objects_;
}; // class Field

#endif // _FIELD_H_

