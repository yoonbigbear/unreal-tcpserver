#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "pch.h"
#include "object/game_object.h"

class Character : public GameObject
{
public:
    Character() = delete;
    Character(uint32_t obj_id, int char_class);

    int map_id() { return map_id_; }
    void map_id(int map_id) { map_id_ = map_id; }

    auto transform() { return transform_; }
    void transform(TransformPtr transform) { transform_ = transform; }

    virtual void Update() override;

private:

    int map_id_ = 0;
    character_class char_class_;

    TransformPtr transform_;

};



#endif // !_CHARACTER_H_


