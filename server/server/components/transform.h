#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "pch.h"

struct Rotation
{
};

struct Position
{
    Position()
    :x(0), y(0), z(0) {}
    Position(float x, float y, float z)
        :x(x), y(y), z(z) {}

    // SIMD 가속화 필요
    Position operator +(Position pos) {
        return Position(x + pos.x, y + pos.y, z + pos.z);
    }
    void operator +=(Position pos) {
        x += pos.x;
        y += pos.y;
        z += pos.z;
    }

    Position operator -(Position pos) {
        return Position(x - pos.x, y - pos.y, z - pos.z);
    }
    void operator -=(Position pos) {
        x -= pos.x;
        y -= pos.y;
        z -= pos.z;
    }

    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

class Transform
{
public:
    
    //Position& position() { return position_; }
    //void position(Position position) { position_ = position; }
    //void position(Position&& position) { position_ = position; }
    //void position(float x, float y, float z) { position_.x = x, position_.y = y, position_.z = z; }
    //void position(float x, float y) { position_.x = x, position_.y = y; }

    //auto dest() { return dest_; }
    //void dest(Position dest) { dest_ = dest; }

    b2Vec3& position() { return position_; }
    void position(b2Vec3 position) { position_ = position; }
    void position(b2Vec3&& position) { position_ = position; }
    void position(float x, float y, float z) { position_.x = x, position_.y = y, position_.z = z; }
    void position(float x, float y) { position_.x = x, position_.y = y; }

    auto dest() { return dest_; }
    void dest(b2Vec3 dest) { dest_ = dest; }

public:

    bool MoveTo();

private:

    //Position position_;
    //Position dest_;
    b2Vec3 position_;
    b2Vec3 dest_;

};


#endif