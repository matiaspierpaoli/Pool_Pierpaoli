#include "gameManager.h"

int main()
{
	GameManager* gameManager = new GameManager();
	;
	gameManager->start();
	delete gameManager;
	return 0;
}