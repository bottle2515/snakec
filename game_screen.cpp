#include "snake_game.h"
#include "defines.h"

// Game::Screen::
void Game::Screen::drawGameField(Game &cGame, char **game, const int gameFieldH, const int gameFieldW, Snake &snake, Food &food) // notice h w
{
	if(cGame.getBlockArrowKey())
		cGame.setBlockArrowKey(false);
	
	int gameFieldPosX = 1,
		gameFieldPosY = 1;
	
	//clean the game field with dots
	for(int i = 0; i < gameFieldH; i++)
	{
		for(int j = 0; j < gameFieldW; j++)
		{
			game[i][j] = '.';
		}
	}
	
	// Draw the food
	game[gameFieldH - 1 - food.y][food.x] = food.symbol;
	
	// Draw the snake
	/* DONE (roy4801#1#): Add multi-length snake support */
	for(Snake::Snake_Body *a = snake.b_head; a->next != NULL; a = a->next)
	{
		int tempX = a->x, tempY = a->y;
		if(tempX == -1 && tempY == -1) // prevent current
		{
			break;
		}
		else if(tempX > gameFieldW - 1 || tempX < 0 || tempY > gameFieldH - 1 || tempY < 0) // if the pos of snake's head exceeds the border will cause read a invaild index from game array
		{
			continue;
		}
		game[gameFieldH - 1 - tempY][tempX] = a == snake.b_head ? snake.symbol : '#';
	}
	
	// Copy gamefiled to screen buffer
	for(int i = 0; i < gameFieldH; i++)
	{
		for(int j = 0; j < gameFieldW; j++)
		{
			_buffer[i + gameFieldPosY][j + gameFieldPosX] = game[i][j];
		}
	}
	// Draw frame to buffer
	_buffer[0][0] = _buffer[0][screenW - 1] = _buffer[screenH - 1][0]
	= _buffer[screenH - 1][screenW - 1] = '+';
	for(int i = 0; i < gameFieldW; i++)
	{
		_buffer[0][i + 1] = _buffer[screenH - 1][i + 1] = '-';
	}
	for(int i = 0; i < gameFieldH; i++)
	{
		_buffer[i + 1][0] = _buffer[i + 1][screenW - 1] = '|';
	}
}

void Game::Screen::setBufferSize(const int screenh, const int screenw)
{
	_buffer = new char *[screenh];
	for(int i = 0; i < screenh; i++)
	{
		_buffer[i] = new char[screenw];
	}
	screenH = screenh;
	screenW = screenw;
	#if DEBUG
	arrayPos(screenW, 0);
	printf("Set screen buffer h=%d w=%d (Game::Screen::setScreenBufferSize)", screenh, screenw);
	#endif
}

void Game::Screen::flipScreen()
{
	//arrayPos(0, 0);
	for(int i = 0; i < screenH; i++)
	{
		arrayPos(0, i);
		for(int a = 0; a < screenW; a++)
		{
			printf("%c", _buffer[i][a]);
		}
	}
	//for(int i = 0; i < screenW; i++)
	//	printf("%c", _buffer[screenH - 1][i]);
	
}
