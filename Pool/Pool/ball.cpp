#include "Ball.h"

Ball::Ball(Vector2 position, Color color, TypeOfBall type, int id)
{
	this->position = position;
	this->color = color;
	this->type = type;
	this->id = id;
	acceleration = { 0 };
	velocity = { 0 };
	direction = { 0 };
	onGame = true;
	isMoving = false;
}

Ball::~Ball()
{

}

Vector2 Ball::GetPosition()
{
	return position;
}

void Ball::SetPosition(Vector2 _position)
{
	this->position.x = _position.x;
	this->position.y = _position.y;
}

Vector2 Ball::GetVelocity()
{
	return velocity;
}

void Ball::SetVelocity(Vector2 _velocity)
{
	this->velocity.x = _velocity.x;
	this->velocity.y = _velocity.y;
}

Vector2 Ball::GetDirection()
{
	return direction;
}

void Ball::SetDirection(Vector2 _direction)
{
	this->direction.x = _direction.x;
	this->direction.y = _direction.y;
}

Color Ball::GetColor()
{
	return color;
}

bool Ball::GetOnGame()
{
	return onGame;
}

void Ball::SetOnGame(bool _onGame)
{
	this->onGame = _onGame;
}

bool Ball::GetIsMoving()
{
	return isMoving;
}

void Ball::SetIsMoving(bool _isMoving)
{
	this->isMoving = _isMoving;
}

void Ball::Draw()
{
	DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<float>(radius), color);
}


void Ball::Hit(Vector2 mousePosition)
{
	Vector2 forceOfHit = { (position.x - mousePosition.x) * 2,(position.y - mousePosition.y) * 2 }; //La fuerza es el doble de la diferencia entre la posición del mouse y la pelota
	if (forceOfHit.x > 1280 || forceOfHit.y > 720)
	{
		forceOfHit = { 1280,720 };
	}
	SetVelocity(forceOfHit);
}

TypeOfBall Ball::GetType()
{
	return type;
}

Vector2 Ball::GetAcceleration()
{
	return acceleration;
}

void Ball::SetAcceleration(Vector2 _acceleration)
{
	this->acceleration = _acceleration;
}

void Ball::Move(Vector2 _movement)
{
	position.x += _movement.x;
	position.y += _movement.y;
}

int Ball::GetID()
{
	return id;
}