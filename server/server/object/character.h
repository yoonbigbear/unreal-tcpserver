#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "game_object.h"

#include "components/transform.h"

class Character : public GameObject
{
public:
    Character(unsigned long long char_id, int char_class);

    int map_id() { return map_id_; }
    void map_id(int map_id) { map_id_ = map_id; }

    TransformPtr transform() { return transform_; }

private:

    int map_id_ = 0;
    character_class char_class_;

    TransformPtr transform_;
};

using CharacterPtr = std::shared_ptr<Character>;

#endif // !_CHARACTER_H_


