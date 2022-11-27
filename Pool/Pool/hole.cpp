#include "Hole.h"

Hole::Hole(Vector2 position, int radius)
{
    this->position.x = position.x;
    this->position.y = position.y;
    this->radius = radius;
}

Hole::~Hole()
{

}

Vector2 Hole::GetPosition()
{
    return position;
}

void Hole::SetPosition(Vector2 _position)
{
    this->position.x = _position.x;
    this->position.y = _position.y;
}

int Hole::GetRadius()
{
    return radius;
}

void Hole::Draw()
{
    DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<float>(radius), BLACK);
}