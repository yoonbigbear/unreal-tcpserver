#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <memory>
#include "../enums.h"

class Character
{
public:
    using Shared = std::shared_ptr<Character>;
    using Weak = std::weak_ptr<Character>;

    character_class class_id_;
    unsigned long long id_;
};

#endif // !_CHARACTER_H_


