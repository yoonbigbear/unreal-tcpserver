#include "field.h"
#include "zone.h"

void Field::Enter(GameObject::Shared obj)
{
    lock_.lock();
    objects_.emplace(obj->obj_id(), obj);
    lock_.unlock();
}

void Field::Leave(uint64_t obj_id)
{
    lock_.lock();
    objects_.erase(obj_id);
    lock_.unlock();
}

void Field::Update()
{
    lock_.lock();
    for (auto e : objects_)
    {
        LOG_INFO("id {}",e.second->obj_id());
    }
    lock_.unlock();
}
