#include "field.h"
#include "zone.h"

#include "message.h"
#include <filesystem>
//#include <navigation/pathfinder_grid.h>
#include <navigation/navigation.h>
#include <world_generated.h>
#include <common_generated.h>

#include "components/transform.h"
#include <protocol_generated.h>

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

    std::vector<Vec3> obj_pos_arr;
    obj_pos_arr.reserve(objects_.size());
    std::vector<uint32_t> obj_id_arr;
    obj_id_arr.reserve(objects_.size());
    for (auto& e : objects_)
    {
        obj_pos_arr.push_back(Vec3(e.second->transform()->position().x,
            e.second->transform()->position().y,
            e.second->transform()->position().z));
        obj_id_arr.push_back(e.second->obj_id());
    }

    net::Message<Protocol, flatbuffers::FlatBufferBuilder> pkt;
    pkt.header.id = Protocol_EnterFieldSync;
    flatbuffer fbb(1024);
    auto builder = world::CreateEnterFieldSyncDirect(fbb, &obj_id_arr, &obj_pos_arr);
    fbb.Finish(builder);
    pkt << fbb;
    Broadcast(pkt);

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
    auto list = players_;
    for (const auto& e : list)
    {
        e.second->session()->Send(msg);
    }
}
