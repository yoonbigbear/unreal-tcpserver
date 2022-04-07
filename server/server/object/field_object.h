#ifndef _FIELD_OBJECT_H_

#include "pch.h"

struct Position
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};
class FieldObject
{
public:
    using Shared = std::shared_ptr<FieldObject>;
    using Weak = std::weak_ptr<FieldObject>;

    FieldObject() = delete;

public:

    Position& GetPosition() { return position_; }
    Position& SetPosition() { return position_; }

private:

    Position position_;
};

#endif // !_FIELD_OBJECT_H_

