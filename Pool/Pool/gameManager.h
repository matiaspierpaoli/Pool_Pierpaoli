#pragma once

#include "Game.h"
class GameManager
{
private:
	Game* game;

public:
	GameManager();
	~GameManager();
	void start();
};