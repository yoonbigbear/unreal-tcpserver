#ifndef _FIELD_H_
#define _FIELD_H_

#include "pch.h"
#include "object/game_object.h"

class Field
{
public:

    using Shared = std::shared_ptr<Field>;
    using Weak = std::weak_ptr<Field>;

    void Enter(GameObject::Shared obj);
    void Leave(uint64_t obj_id);

    void Update();

    template<typename message>
    void Broadcast(message msg);
private:

    std::mutex lock_;

    std::unordered_map<uint64_t, GameObject::Shared> objects_;
    std::unordered_map<uint64_t, GameObject::Shared> players_;

}; // class Field

#endif // _FIELD_H_

