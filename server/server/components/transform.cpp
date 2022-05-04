#include "transform.h"
#include "timer.h"

bool Transform::MoveTo()
{
    auto length = b2Vec2(dest_.x - position_.x, dest_.y - position_.y).LengthSquared();
    if (length < 5 * 5)
    {
        return false;
    }

    position_.x += 0.1;
    position_.y += 0.1;
    LOG_INFO("position : {}", position_.x);

    return true;
}
