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
    Character(uint64_t char_id, int char_class);

private:
    std::shared_ptr<FieldObject> field_object_;

    character_class char_class_;
    unsigned long long char_id_;
};

#endif // !_CHARACTER_H_


