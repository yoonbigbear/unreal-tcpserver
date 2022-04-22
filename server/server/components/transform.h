#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <memory>

struct Rotation
{
};

struct Position
{
    Position()
    :x(0), y(0), z(0) {}
    Position(float x, float y, float z)
        :x(x), y(y), z(z) {}

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

class Transform
{
public:

    Position& position() { return position_; }
    void position(Position position) { position_ = position; }
    void position(Position&& position) { position_ = position; }
    void position(float x, float y, float z) 
    {
        position_.x = x;
        position_.y = y;
        position_.z = z;
    }
    void position(float x, float y) { position(x, y, position_.z); }

private:
    Position position_;
};

using TransformPtr = std::shared_ptr<Transform>;
#endif