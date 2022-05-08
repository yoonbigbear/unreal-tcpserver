#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "pch.h"


#ifdef RPG3D
struct Rotation
{
};
struct Position : public b2Vec3
{
    b2Vec2&& operator +(b2Vec2&& pos2d)
    {
        return b2Vec2(x + pos2d.x, y + pos2d.y);
    }
    void operator +=(b2Vec2&& pos2d)
    {
        x += pos2d.x;
        y += pos2d.y;
    }
    b2Vec2&& operator -(b2Vec2&& pos2d)
    {
        return b2Vec2(x - pos2d.x, y - pos2d.y);
    }
    void operator -=(b2Vec2&& pos2d)
    {
        x -= pos2d.x;
        y -= pos2d.y;
    }

    void operator =(b2Vec2&& pos2d)
    {
        x = pos2d.x;
        y = pos2d.y;
    }
    void operator =(b2Vec2& pos2d)
    {
        x = pos2d.x;
        y = pos2d.y;
    }

};


class Transform
{
public:

    Position& position() { return position_; }
    void position(Position&& position) { position_ = position; }
    void position(Position& position) { position_ = position; }
    void position(b2Vec2&& position) { position_.x = position.x; position_.y = position.y; }
    void position(b2Vec2& position) { position_.x = position.x; position_.y = position.y; }
    void position(float x, float y, float z) { position_.x = x, position_.y = y, position_.z = z; }
    void position(float x, float y) { position_.x = x, position_.y = y; }

    Position& dest() { return dest_; }
    void dest(Position&& dest) { dest_ = dest; }
    void dest(Position& dest) { dest_ = dest; }
    void dest(b2Vec2&& dest) { dest_ = dest; }
    void dest(b2Vec2& dest) { dest_ = dest; }

public:

    bool MoveTo();

private:

    Position position_;
    Position dest_;

};
#else
struct Position
{
    int x_;
    int y_;
};
class Transform
{
public:
    Position& position() { return position_; }

private:
    Position position_;
};
#endif


#endif