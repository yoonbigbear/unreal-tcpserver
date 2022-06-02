#ifndef _FIELD_H_
#define _FIELD_H_

#include "pch.h"
#include "object/game_object.h"
#include <box2d/box2d.h>

class World
{
public:

    World();
    ~World();

    void Start(const char* path);

    void Enter(GameObjectPtr obj);
    void Leave(uint64_t obj_id);

    void Update(float dt);

    template<typename message>
    void Broadcast(message msg);

    class Navigation* navigation() { return navigation_; }
    //class PathFinderGrid* grids() { return grids_; }
private:

    std::mutex lock_;

    std::unordered_map<uint64_t, GameObjectPtr> objects_;
    std::unordered_map<uint64_t, GameObjectPtr> players_;
    Navigation* navigation_ = nullptr;

    //PathFinderGrid* grids_ = nullptr;


}; // class Field

#endif // _FIELD_H_

