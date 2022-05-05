#include "field.h"
#include "zone.h"

#include "message.h"
#include <filesystem>
#include <navigation/navigation.h>

Field::Field()
{
    Start("");
}

Field::~Field()
{
    if (navigation_)
    {
        delete navigation_;
        navigation_ = nullptr;
    }
}

void Field::Start(const char* path)
{
    std::string nav_path = std::filesystem::current_path().string() + "\\Plane.bin";

    if (!navigation_)
        navigation_ = new Navigation();

    navigation_->navigation(navigation_->LoadAll(nav_path.c_str()));
    navigation_->query()->init(navigation_->navigation(), navigation_->navigation()->getMaxTiles());
    float p[3] = {};
    navigation_->RandomPoint(p);
    if (p)
    {

    }
}

void Field::Enter(GameObjectPtr obj)
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
        auto gameobject = e.second;
        if (!gameobject)
            continue;

        gameobject->Update();
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
