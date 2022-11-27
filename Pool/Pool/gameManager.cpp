#include <iostream>

#include "gameManager.h"

GameManager::GameManager()
{
	InitWindow(1280, 720, "Pool");
	game = new Game();
}

GameManager::~GameManager()
{
	game->DeInit();
	delete game;
	CloseWindow();
}

void GameManager::start()
{
	game->Init();

	while (!WindowShouldClose())
	{
		game->Input();
		game->Update();
		game->Draw();
	}
}