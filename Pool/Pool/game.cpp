#include "Game.h"

Game::Game()
{
	strippedBallsTotal;
	playerOneTurn = true;
	playerOneWon = false;
	playerTwoWon = false;
	allBallsStill = true;
	whiteBallHit = false;
	gameOver = false;
	ballsOnGame = 16;
}

Game::~Game()
{

}

void Game::BallBallCollision(vector<Ball*> _balls)
{
	for (auto& ball : balls) 
	{
		//Se resta, ya que de otra forma las bolas nunca frenarian y mantendrian una velocidad constante o incremental
		ball->SetAcceleration({ -(ball->GetVelocity().x * 0.8f + friction * GetFrameTime() + airFriction * GetFrameTime()),-(ball->GetVelocity().y * 0.8f + friction * GetFrameTime() + airFriction * GetFrameTime()) });

		// Fisicas de las balls
		// Se setea la velocidad de la bola
		ball->SetVelocity({ ball->GetVelocity().x + ball->GetAcceleration().x * GetFrameTime(), ball->GetVelocity().y + ball->GetAcceleration().y * GetFrameTime() });

		//Se setea su posicion
		ball->SetPosition({ ball->GetPosition().x + ball->GetVelocity().x * GetFrameTime(), ball->GetPosition().y + ball->GetVelocity().y * GetFrameTime() });

		// Si la velocidad de la bola en X o en Y es menor que 1 entonces seteo todo en 0. Fabs me devuelve un valor absoluto
		if (fabs(ball->GetVelocity().x * ball->GetVelocity().x + ball->GetVelocity().y * ball->GetVelocity().y) < 1) 
		{
			ball->SetVelocity({ 0,0 });
			ball->SetAcceleration({ 0,0 });
		}
	}

	for (auto& ball : balls) 
	{
		for (auto& target : balls) 
		{
			if (ball->GetID() != target->GetID() && ball->GetOnGame() && target->GetOnGame())
			{
				//Si la distancia entre ambas es menor o igual al radio por 2, significa que colisionan  PITAGORAS
				if (sqrt(pow(ball->GetPosition().x - target->GetPosition().x, 2) + pow(ball->GetPosition().y - target->GetPosition().y, 2)) <= (double)radius * 2)
				{
					CollidingBalls.push_back({ ball, target });

					//Lo uso para que las bolas no se queden overlapeadas, de esta manera chocan en su radio
					float distance = sqrtf(powf(ball->GetPosition().x - target->GetPosition().x, 2) + powf(ball->GetPosition().y - target->GetPosition().y, 2));
					float overlap = 0.5f * (distance - radius * 2);

					//Les seteo nuevamente su posicion
					ball->SetPosition({ ball->GetPosition().x - overlap * (ball->GetPosition().x - target->GetPosition().x) / distance, ball->GetPosition().y - overlap * (ball->GetPosition().y - target->GetPosition().y) / distance });
					target->SetPosition({ target->GetPosition().x + overlap * (ball->GetPosition().x - target->GetPosition().x) / distance, target->GetPosition().y + overlap * (ball->GetPosition().y - target->GetPosition().y) / distance });
				}
			}
		}
	}

	for (auto& collision : CollidingBalls)
	{
		Ball* ball1 = collision.first;
		Ball* ball2 = collision.second;
		
		float ballsDistance = sqrtf(powf(ball2->GetPosition().x - ball1->GetPosition().x, 2) + powf(ball2->GetPosition().y - ball1->GetPosition().y, 2));

		float normalX = (ball2->GetPosition().x - ball1->GetPosition().x) / ballsDistance;
		float normalY = (ball2->GetPosition().y - ball1->GetPosition().y) / ballsDistance;

		float tangentX = -normalY;
		float tangentY = normalX;

		float dotProductTangent1 = ball1->GetVelocity().x * tangentX + ball1->GetVelocity().y * tangentY;
		float dotProductTangent2 = ball2->GetVelocity().x * tangentX + ball2->GetVelocity().y * tangentY;

		float dotProductNormal1 = ball1->GetVelocity().x * normalX + ball1->GetVelocity().y * normalY;
		float dotProductNormal2 = ball2->GetVelocity().x * normalX + ball2->GetVelocity().y * normalY;

		float momentumConservation1 = (dotProductNormal1 / (mass * 2)) + dotProductNormal2;
		float momentumConservation2 = (dotProductNormal2 / (mass * 2)) + dotProductNormal1;

		ball1->SetVelocity({ tangentX * dotProductTangent1 + normalX * momentumConservation1, tangentY * dotProductTangent1 + normalY * momentumConservation1 });
		ball2->SetVelocity({ tangentX * dotProductTangent2 + normalX * momentumConservation2, tangentY * dotProductTangent2 + normalY * momentumConservation2 });

	}
	CollidingBalls.clear();
}

//Collision Circle Rect
//https://ants.inf.um.es/staff/jlaguna/tp/tutoriales/colisiones/index.html

void Game::BorderBallCollision(vector<Border*> _borders, Ball* _ball) 
{
	for (unsigned int i = 0; i < borders.size(); i++) 
	{
		//Dependiendo el borde con el que colisione
		if (borders[i]->GetBorderPosition() == BorderPosition::UPLEFT) 
		{
			//Simple colision Circulo Rectangulo
			if (_ball->GetPosition().x + radius >= borders[i]->GetBorderRec().x && _ball->GetPosition().x - radius <= borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width && _ball->GetPosition().y - radius <= borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height) 
			{
				//Cambia la trayectoria de la pelota y evita que se vaya de la mesa
				_ball->SetPosition({ _ball->GetPosition().x, borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height + radius });
				_ball->SetVelocity({ _ball->GetVelocity().x / 2, -_ball->GetVelocity().y / 2 });
			}
		}
		else if (borders[i]->GetBorderPosition() == BorderPosition::UPRIGHT) 
		{
			if (_ball->GetPosition().x + radius >= borders[i]->GetBorderRec().x && _ball->GetPosition().x - radius <= borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width && _ball->GetPosition().y - radius <= borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height) 
			{
				_ball->SetPosition({ _ball->GetPosition().x, borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height + radius });
				_ball->SetVelocity({ _ball->GetVelocity().x / 2, -_ball->GetVelocity().y / 2 });
			}
		}
		else if (borders[i]->GetBorderPosition() == BorderPosition::DOWNLEFT) 
		{
			if (_ball->GetPosition().x + radius >= borders[i]->GetBorderRec().x && _ball->GetPosition().x - radius <= borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width && _ball->GetPosition().y + radius >= borders[i]->GetBorderRec().y) 
			{
				_ball->SetPosition({ _ball->GetPosition().x, borders[i]->GetBorderRec().y - radius });
				_ball->SetVelocity({ _ball->GetVelocity().x / 2, -_ball->GetVelocity().y / 2 });
			}
		}
		else if (borders[i]->GetBorderPosition() == BorderPosition::DOWNRIGHT) 
		{
			if (_ball->GetPosition().x + radius >= borders[i]->GetBorderRec().x && _ball->GetPosition().x - radius <= borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width && _ball->GetPosition().y + radius >= borders[i]->GetBorderRec().y) 
			{
				_ball->SetPosition({ _ball->GetPosition().x, borders[i]->GetBorderRec().y - radius });
				_ball->SetVelocity({ _ball->GetVelocity().x / 2 , -_ball->GetVelocity().y / 2 });
			}
		}
		else if (borders[i]->GetBorderPosition() == BorderPosition::RIGHT) 
		{
			if (_ball->GetPosition().x + radius >= borders[i]->GetBorderRec().x && _ball->GetPosition().y + radius >= borders[i]->GetBorderRec().y && _ball->GetPosition().y - radius <= borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height) 
			{
				_ball->SetPosition({ _ball->GetPosition().x - radius , _ball->GetPosition().y });
				_ball->SetVelocity({ -_ball->GetVelocity().x / 2, _ball->GetVelocity().y / 2 });
			}
		}
		else if (borders[i]->GetBorderPosition() == BorderPosition::LEFT) 
		{
			if (_ball->GetPosition().x - radius <= borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width && _ball->GetPosition().y - radius >= borders[i]->GetBorderRec().y && _ball->GetPosition().y - radius <= borders[i]->GetBorderRec().y + borders[i]->GetBorderRec().height) 
			{
				_ball->SetPosition({ borders[i]->GetBorderRec().x + borders[i]->GetBorderRec().width + radius , _ball->GetPosition().y });
				_ball->SetVelocity({ -_ball->GetVelocity().x / 2 , _ball->GetVelocity().y / 2 });
			}
		}
	}
}


void Game::HoleBallCollision(vector<Hole*> _holes, Ball* _ball)
{
	for (unsigned int i = 0; i < holes.size(); i++)
	{
		//Pitagoras para saber la distancia entra la bola y el hoyo
		if (sqrt(pow(holes[i]->GetPosition().x - _ball->GetPosition().x, 2) + pow(holes[i]->GetPosition().y - _ball->GetPosition().y, 2)) < (holes[i]->GetRadius() + radius) / 2)
		{
			//Si la bola que entra es blanca entonces vuelve a la posicion Inicial
			if (_ball->GetType() == TypeOfBall::WHITEBALL)
			{
				_ball->SetPosition({ screenWidth * 2 / 10, screenHeight / 2 });
				_ball->SetDirection({ 0, 0 });
				_ball->SetVelocity({ 0, 0 });
				_ball->SetAcceleration({ 0 });
			}
			//Si la bola es negra y no metio todas las pelotas entonces pierde
			if (_ball->GetType() == TypeOfBall::BLACKBALL)
			{
				//Creo variables para hacer el recuento de bolas
				int counterStriped = 0;
				int counterSmooth = 0;

				//Cuento las bolas y de que tipo son
				for (unsigned int j = 0; j < balls.size(); j++)
				{
					if (!balls[j]->GetOnGame() && balls[j]->GetType() == TypeOfBall::REDBALL)
					{
						counterStriped++;
					}
					if (!balls[j]->GetOnGame() && balls[j]->GetType() == TypeOfBall::BLUEBALL)
					{
						counterSmooth++;
					}
				}
				
				//Compruebo el turno de quien era
				if (playerOneTurn)
				{
					//Si era el turno del jugador 1 y la cantida de bolas metidas es igual al total entonces gano, en caso contrario gana el jugador 2
					if (counterStriped == strippedBallsTotal)
					{
						playerOneWon = true;
					}
					else if (counterStriped < strippedBallsTotal)
					{
						playerTwoWon = true;
					}
				}
				else
				{
					if (counterSmooth == strippedBallsTotal)
					{
						playerTwoWon = true;
					}
					else if (counterSmooth < strippedBallsTotal)
					{
						playerOneWon = true;
					}
				}
				gameOver = true;
			}

			//Si metieron una bola entonces cambia de turno, dependiendo sea el caso
			if (_ball->GetType() == TypeOfBall::REDBALL)
			{
				if (!playerOneTurn)
				{
					playerOneTurn = true;
				}
				_ball->SetOnGame(false);
			}
			if (_ball->GetType() == TypeOfBall::BLUEBALL)
			{
				if (playerOneTurn)
				{
					playerOneTurn = false;
				}
				_ball->SetOnGame(false);
			}
		}

	}
}

//Checkea las bolas en juego
int Game::CheckBalls(vector<Ball*> _balls)
{
	int counter = 0;
	for (unsigned int i = 0; i < balls.size(); i++)
	{
		if (balls[i]->GetOnGame())
		{
			counter++;
		}
	}
	return counter;
}

void Game::Update()
{
	if (!gameOver)
	{
		//Recuento de bolas en juego
		int previousBallsOnGame = CheckBalls(balls);

		//Bolas en movimiento
		int ballsStill = 0;

		//Llamado a las colisiones
		for (unsigned int i = 0; i < balls.size(); i++) 
		{
			BorderBallCollision(borders, balls[i]);
			HoleBallCollision(holes, balls[i]);
		}
		BallBallCollision(balls);

		//Velocida de las bolas en juego, ya que queremos que todas las bolas se paren antes de poder seguir
		for (unsigned int i = 0; i < balls.size(); i++)
		{
			if (balls[i]->GetVelocity().x == 0 && balls[i]->GetVelocity().y == 0)
			{
				ballsStill++;
			}
		}

		//Cuento las bolas para saber si estan todas quietas
		if (ballsStill == 16)
		{
			allBallsStill = true;
		}
		else
		{
			allBallsStill = false;
		}
		ballsOnGame = CheckBalls(balls);

		//Si hay bolas en juego y la bola blanca no esta siendo golpeada y todas las bolas estan en velocidad 0, entonces cambio de turno
		if (previousBallsOnGame == ballsOnGame && whiteBallHit && allBallsStill)
		{
			playerOneTurn = !playerOneTurn;
			whiteBallHit = false;
		}
	}
}

void Game::Draw()
{
	BeginDrawing();

	if (!gameOver)
	{
		ClearBackground(DARKGREEN);
		for (unsigned int i = 0; i < holes.size(); i++)
		{
			holes[i]->Draw();
		}
		for (unsigned int i = 0; i < borders.size(); i++)
		{
			borders[i]->Draw();
		}
		for (unsigned int i = 0; i < balls.size(); i++)
		{
			if (balls[i]->GetOnGame()) {
				balls[i]->Draw();
			}
		}
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !whiteBallHit) 
		{
			DrawLineEx({ GetMousePosition().x, GetMousePosition().y }, { balls[0]->GetPosition().x, balls[0]->GetPosition().y }, 10, BLACK);
		}
		if (playerOneTurn)
		{
			DrawText("P1", screenWidth / 4 - 60, screenHeight / 4 - 180, 40, BLACK);
		}
		else
		{
			DrawText("P2", screenWidth / 4 - 60, screenHeight / 4 - 180, 40, BLACK);
		}
	}

	EndDrawing();
}

void Game::Init()
{
	playerOneTurn = true;
	playerOneWon = false;
	playerTwoWon = false;
	allBallsStill = true;
	whiteBallHit = false;
	gameOver = false;
	holes.push_back(new Hole({ 36,36 }, 36));
	holes.push_back(new Hole({ 36,screenHeight - 36 }, 36));
	holes.push_back(new Hole({ screenWidth - 36,36 }, 36));
	holes.push_back(new Hole({ screenWidth - 36,screenHeight - 36 }, 36));
	holes.push_back(new Hole({ screenWidth / 2,screenHeight - 36 }, 36));
	holes.push_back(new Hole({ screenWidth / 2, 36 }, 36));
	Rectangle upLeft;
	upLeft.x = 72;
	upLeft.y = 0;
	upLeft.width = screenWidth / 2 - 108;
	upLeft.height = 32;
	Rectangle upRight;
	upRight.x = screenWidth / 2 + 36;
	upRight.y = 0;
	upRight.width = screenWidth / 2 - 108;
	upRight.height = 32;
	Rectangle downLeft;
	downLeft.x = 72;
	downLeft.y = screenHeight - 36;
	downLeft.width = screenWidth / 2 - 108;
	downLeft.height = 32;
	Rectangle downRight;
	downRight.x = screenWidth / 2 + 36;
	downRight.y = screenHeight - 36;
	downRight.width = screenWidth / 2 - 108;
	downRight.height = 32;
	Rectangle left;
	left.x = 0;
	left.y = 72;
	left.width = 30;
	left.height = screenHeight - 144;
	Rectangle right;
	right.x = screenWidth - 30;
	right.y = 72;
	right.width = 30;
	right.height = screenHeight - 144;
	borders.push_back(new Border(BorderPosition::UPLEFT, upLeft));
	borders.push_back(new Border(BorderPosition::UPRIGHT, upRight));
	borders.push_back(new Border(BorderPosition::DOWNLEFT, downLeft));
	borders.push_back(new Border(BorderPosition::DOWNRIGHT, downRight));
	borders.push_back(new Border(BorderPosition::LEFT, left));
	borders.push_back(new Border(BorderPosition::RIGHT, right));
	balls.push_back(new Ball({ (screenWidth * 2 / 10),(screenHeight / 2) }, WHITE, TypeOfBall::WHITEBALL, 0));
	balls.push_back(new Ball({ (screenWidth * 7 / 10),(screenHeight * 5 / 10) }, RED, TypeOfBall::REDBALL, 1));
	balls.push_back(new Ball({ (screenWidth * 7.4f / 10),screenHeight * 4.6f / 10 }, RED, TypeOfBall::REDBALL, 2));
	balls.push_back(new Ball({ screenWidth * 7.4f / 10,screenHeight * 5.4f / 10 }, BLUE, TypeOfBall::BLUEBALL, 3));
	balls.push_back(new Ball({ screenWidth * 7.8f / 10,screenHeight * 4.2f / 10 }, BLUE, TypeOfBall::BLUEBALL, 4));
	balls.push_back(new Ball({ screenWidth * 7.8f / 10,screenHeight * 5 / 10 }, BLACK, TypeOfBall::BLACKBALL, 5));
	balls.push_back(new Ball({ screenWidth * 7.8f / 10,screenHeight * 5.8f / 10 }, RED, TypeOfBall::REDBALL, 6));
	balls.push_back(new Ball({ screenWidth * 8.2f / 10,screenHeight * 3.5f / 10 }, RED, TypeOfBall::REDBALL, 7));
	balls.push_back(new Ball({ screenWidth * 8.2f / 10,screenHeight * 4.5f / 10 }, BLUE, TypeOfBall::BLUEBALL, 8));
	balls.push_back(new Ball({ screenWidth * 8.2f / 10,screenHeight * 5.5f / 10 }, RED, TypeOfBall::REDBALL, 9));
	balls.push_back(new Ball({ screenWidth * 8.2f / 10,screenHeight * 6.5f / 10 }, BLUE, TypeOfBall::BLUEBALL, 10));
	balls.push_back(new Ball({ screenWidth * 8.6f / 10,screenHeight * 3 / 10 }, BLUE, TypeOfBall::BLUEBALL, 11));
	balls.push_back(new Ball({ screenWidth * 8.6f / 10,screenHeight * 4 / 10 }, RED, TypeOfBall::REDBALL, 12));
	balls.push_back(new Ball({ screenWidth * 8.6f / 10,screenHeight * 5 / 10 }, BLUE, TypeOfBall::BLUEBALL, 13));
	balls.push_back(new Ball({ screenWidth * 8.6f / 10,screenHeight * 6 / 10 }, BLUE, TypeOfBall::BLUEBALL, 14));
	balls.push_back(new Ball({ screenWidth * 8.6f / 10,screenHeight * 7 / 10 }, RED, TypeOfBall::REDBALL, 15));
}

void Game::Input()
{
	Vector2 mousePosition = GetMousePosition();

	//Si presiono el boton izquierdo y la velocidad de las bolas es igual a 0 entonces puedo llamar a HIT()
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && balls[0]->GetVelocity().x == 0 && balls[0]->GetVelocity().y == 0 && allBallsStill)
	{
		balls[0]->Hit(mousePosition);
		whiteBallHit = true;
	}
}

void Game::DeInit()
{
	for (auto&& ball : balls)
	{
		delete ball;
	}
	balls.clear();
	for (auto&& hole : holes)
	{
		delete hole;
	}
	holes.clear();
	for (auto&& border : borders)
	{
		delete border;
	}
	borders.clear();
}