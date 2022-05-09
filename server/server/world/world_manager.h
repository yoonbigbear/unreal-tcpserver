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

    void Start();

    void EnterField(int field_id, GameObjectPtr obj);
    void LeaveField();

    void CreateSampleAI(int field_id, int count);

    void Update(float dt);


    FieldPtr field(int id) { return fields_[id]; }

private:
    std::unordered_map<unsigned int, FieldPtr> fields_;

    int ai_id_count_ = 0;

}; // class Field

#endif // _WORLD_MANAGER_H_