#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "pch.h"

class FieldObject;
class Character
{
public:
    using Shared = std::shared_ptr<Character>;
    using Weak = std::weak_ptr<Character>;

    Character() = delete;

private:
    std::shared_ptr<FieldObject> field_object_;

    character_class class_type_;
    unsigned long long id_;
};

#endif // !_CHARACTER_H_


