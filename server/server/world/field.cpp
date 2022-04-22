#include "field.h"
#include "zone.h"

void Field::Enter(GameObject::Shared obj)
{
    objects_.emplace(obj->obj_id(), obj);
}

void Field::Leave()
{
}
