#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include <unordered_map>
#include <singleton.h>

#include "field.h"
#include "object/game_object.h"


class WorldManager : public net::Singleton<WorldManager>
{
public:
    WorldManager();
    ~WorldManager();

    void EnterField(int field_id, GameObject::Shared obj);
    void LeaveField();

private:
    std::unordered_map<unsigned int, Field::Shared> fields_;

}; // class Field

#endif // _WORLD_MANAGER_H_