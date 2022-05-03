#include "transform.h"
#include "timer.h"

bool Transform::MoveTo()
{
    
    if ((b2Vec2(dest_.x - position_.x, dest_.y - position_.y).LengthSquared() > 1000))
    {
        return false;
    }

    auto dir = dest_ - position_;

    auto next_pos = b2Vec2(dir.x, dir.y).Normalize() * net::delta_time * 10;

    LOG_INFO("position : {}", next_pos);

    return true;
}
