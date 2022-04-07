#ifndef _FIELD_H_
#define _FIELD_H_

#include "pch.h"
#include "object/field_object.h"

class Field
{
public:

    void EnterField();
    void LeaveField();

private:

    std::unordered_map<int64_t, std::shared_ptr<FieldObject>> objects_;
}; // class Field

#endif // _FIELD_H_

