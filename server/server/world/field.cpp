#include "field.h"
#include "zone.h"

#include "message.h"

void Field::Enter(GameObject::Shared obj)
{
    lock_.lock();
    objects_.emplace(obj->obj_id(), obj);
    if (obj->session())
        players_.emplace(obj->obj_id(), obj);
    lock_.unlock();
}

void Field::Leave(uint64_t obj_id)
{
    lock_.lock();
    objects_.erase(obj_id);
    players_.erase(obj_id);
    lock_.unlock();
}

void Field::Update()
{
    lock_.lock();
    for (const auto& e : objects_)
    {
        LOG_INFO("id {}",e.second->obj_id());
    }
    lock_.unlock();
}

template<typename message>
void Field::Broadcast(message msg)
{
    lock_.lock();
    for (const auto& e : players_)
    {
        e.second->session()->Send(msg);
    }
    lock_.unlock();
}
