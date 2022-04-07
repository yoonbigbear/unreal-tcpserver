#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include <unordered_map>
#include <singleton.h>

#include "field.h"

class WorldManager : public net::Singleton<WorldManager>
{
public:

    void EnterWorld();
    void LeaveWorld();

private:
    std::unordered_map<unsigned int, Field> fields_;

}; // class Field

#endif // _WORLD_MANAGER_H_