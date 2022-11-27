#pragma once

#include "raylib.h"

class Hole
{
private:
	Vector2 position;
	int radius;
public:
	Hole(Vector2 position, int radius);
	~Hole();
	Vector2 GetPosition();
	void SetPosition(Vector2 position);
	int GetRadius();
	void Draw();
};