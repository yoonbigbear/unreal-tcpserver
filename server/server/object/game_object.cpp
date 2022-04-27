#include "game_object.h"

void GameObject::CreateCharacter(int char_class)
{
    character_ = std::make_shared<Character>(char_class);
}

void GameObject::CreateTransform()
{
    transform_ = std::make_shared<Transform>();
}

void GameObject::CreateAI()
{
    ai_ = std::make_shared<AI>();
}
