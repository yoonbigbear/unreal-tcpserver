#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "pch.h"
#include "object/game_object.h"

class Character : public GameObject
{
public:
    Character() = delete;
    Character(uint32_t obj_id, int char_class);

    void start();

    virtual void Update(float dt) override;

private:
    character_class char_class_;

    FieldPtr field_ = nullptr;

};



#endif // !_CHARACTER_H_


