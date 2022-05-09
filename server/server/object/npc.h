#ifndef _NPC_H_
#define _NPC_H_

#include "pch.h"
#include "object/game_object.h"

class Npc : public GameObject
{
public:
    Npc() = delete;
    Npc(uint32_t obj_id);

    auto transform() { return transform_; }
    void transform(TransformPtr transform) { transform_ = transform; }

    auto ai() { return ai_; }
    void ai(AIPtr ai) { ai_ = ai; }

public:
    void start();

    virtual void Update(float dt) override;

private:
    FieldPtr field_ = nullptr;
    TransformPtr transform_;
    AIPtr ai_;

    bool move_ = false;
};



#endif // !_NPC_H_


