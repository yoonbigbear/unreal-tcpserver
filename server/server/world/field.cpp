#include "field.h"
#include "zone.h"

#include "message.h"
#include <filesystem>
//#include <navigation/pathfinder_grid.h>
#include <navigation/navigation.h>

World::World()
{
    Start("");
}

World::~World()
{
    if (navigation_)
    {
        delete navigation_;
        navigation_ = nullptr;
    }

    /*if (grids_)
    {
        delete grids_;
        grids_ = nullptr;
    }*/
}

void World::Start(const char* path)
{
    std::string nav_path = std::filesystem::current_path().string() + "\\Plane.bin";

    if (!navigation_)
        navigation_ = new Navigation();

    navigation_->navigation(navigation_->LoadAll(nav_path.c_str()));
    navigation_->query()->init(navigation_->navigation(), navigation_->navigation()->getMaxTiles());

    //grids_ = new PathFinderGrid(50,50);

}

void World::Enter(GameObjectPtr obj)
{
    lock_.lock();
    objects_.emplace(obj->obj_id(), obj);
    if (obj->session())
        players_.emplace(obj->obj_id(), obj);
    lock_.unlock();
}

void World::Leave(uint64_t obj_id)
{
    lock_.lock();
    objects_.erase(obj_id);
    players_.erase(obj_id);
    lock_.unlock();
}

void World::Update(float dt)
{
    lock_.lock();
    for (const auto& e : objects_)
    {
        auto gameobject = e.second;
        if (!gameobject)
            continue;

        gameobject->Update(dt);
    }
    lock_.unlock();
}

template<typename message>
void World::Broadcast(message msg)
{
    lock_.lock();
    for (const auto& e : players_)
    {
        e.second->session()->Send(msg);
    }
    lock_.unlock();
}
