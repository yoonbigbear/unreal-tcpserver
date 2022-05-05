#ifndef _FIELD_H_
#define _FIELD_H_

#include "pch.h"
#include "object/game_object.h"

class Field
{
public:

    Field();
    ~Field();
    using Shared = std::shared_ptr<Field>;
    using Weak = std::weak_ptr<Field>;

    void Start(const char* path);

    void Enter(GameObjectPtr obj);
    void Leave(uint64_t obj_id);

    void Update();

    template<typename message>
    void Broadcast(message msg);

private:

    std::mutex lock_;

    std::unordered_map<uint64_t, GameObjectPtr> objects_;
    std::unordered_map<uint64_t, GameObjectPtr> players_;
    class Navigation* navigation_ = nullptr;



}; // class Field

#endif // _FIELD_H_

