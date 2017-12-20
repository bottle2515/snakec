#include "snake_game.h"
#include "defines.h"


Game::Game() : _gameField(NULL),
				_gameFieldW(0),
				_gameFieldH(0), 
				_needUpdate(false),
				_pauseUpdate(false),
				_wallType(WALL_CLOSE),
				_blockArrowKey(false)
{
}

Game::~Game()
{
	for(int i = 0; i < _gameFieldH; i++)
	{
		delete [] _gameField[i];
	}
	delete [] _gameField;
}

// A lots of generating things
bool Game::generateGamePlace(const int h, const int w)
{
	if(!_gameField)
	{
		_gameField = new char *[h];
		for(int i = 0; i < h; i++)
		{
			_gameField[i] = new char[w];
			for(int a = 0; a < w; a++)
			{
				_gameField[i][a] = '.';
			}
		}
		_gameFieldW = w;
		_gameFieldH = h;
		return false;
	}
	else // Error
		return true;
}

void Game::printMap()
{
	for(int i = 0; i < _gameFieldH; i++)
	{
		for(int j = 0; j < _gameFieldW; j++)
		{
			printf("%c ", _gameField[i][j]);
		}
		putchar('\n');
	}
}

int Game::getInput()
{
	if(kbhit())
	{
		unsigned char c = getch();
		switch(c)
		{
			case 27:
				return KEY_ESC;
				break;
			case 224: // Arrow Key 
			{
				#if DEBUG
				arrayPos(DEBUG_O, 6);
				printf("Arrow Key:");
				arrayPos(DEBUG_O, 7);
				#endif
				unsigned char c2 = getch();
				switch(c2)
				{
					case ARR_UP:
						#if DEBUG
						printf("press UP");
						#endif
						return KEY_UP;
						break;
					case ARR_DOWN:
						#if DEBUG
						printf("press DOWN");
						#endif
						return KEY_DOWN;
						break;
					case ARR_LEFT:
						#if DEBUG
						printf("press LEFT");
						#endif
						return KEY_LEFT;
						break;
					case ARR_RIGHT:
						#if DEBUG
						printf("press RIGHT");
						#endif
						return KEY_RIGHT;
						break;
				}
			}
			break;
			
			default: // NOT handle
				#if DEBUG
				printf("press NOT_HANDLE");
				#endif
				return KEY_NOTHANDLE;
				break;
		}
	}
	else
		return KEY_NOTPRESS;
}

void Game::processInput()
{
	int key = getInput();
	
	switch(key)
	{
		case KEY_NOTPRESS:
			break;
		case KEY_NOTHANDLE:
			break;
		case KEY_ESC:
			exit(0);
			break;
		//If the arrow key is pressed down means that the snake must be update 
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			if(!_needUpdate)
				_needUpdate = true;
			break;
		default:
			printf("Invaild Key!\n");
			exit(EXIT_FAILURE);
	}
	//Arrow Key Process
	if(!_blockArrowKey && !snake.getDead())// if blcokArrowKey == false
	{
		switch(key)
		{
			case KEY_UP:
				if(snake.getDirection() != DIR_DOWN)
				{
					snake.setDirection(DIR_UP);
					_blockArrowKey = true;
				}
				break;
			case KEY_DOWN:
				if(snake.getDirection() != DIR_UP) 
				{
					snake.setDirection(DIR_DOWN);
					_blockArrowKey = true;
				}
				break;
			case KEY_LEFT:
				if(snake.getDirection() != DIR_RIGHT)
				{
					snake.setDirection(DIR_LEFT);
					_blockArrowKey = true;
				}
				break;
			case KEY_RIGHT:
				if(snake.getDirection() != DIR_LEFT)
				{
					snake.setDirection(DIR_RIGHT);
					_blockArrowKey = true;
				}
				break;
		}
	}
}

void Game::start()
{
	screen.setBufferSize(_gameFieldH + 2, _gameFieldW + 2);

	snake.generate(_gameFieldH, _gameFieldW);

	food.generate(_gameFieldH, _gameFieldW, snake);
	
	clock_t end = 0, start = 0;
	screen.drawGameField(*this, _gameField, _gameFieldH, _gameFieldW, snake, food);
	screen.flipScreen();
	
	_running = true;
	while(_running)
	{
		processInput();
		
		end = clock();
		if(end - start > 300)
		{
			update();

			system("cls");
			screen.drawGameField(*this, _gameField, _gameFieldH, _gameFieldW, snake, food);
			screen.flipScreen();
			
			start = end;
			
			#if DEBUG
				debug_line = 0;
				arrayPos(DEBUG_O, OUTPUT);
				printf("Snake Info:\n");
				arrayPos(DEBUG_O, OUTPUT+1);
				printf("Direction: %d ", snake.direction);
				printf("Length: %d", snake.length);
				arrayPos(DEBUG_O, OUTPUT+2);
				printf("Pos:");
				for(Snake::Snake_Body *i = snake.b_head; i->next != NULL; i = i->next)
				{
					arrayPos(DEBUG_O, OUTPUT + 3 + debug_line++);
					printf("x: %d y: %d\n", i->x, i->y);
				}
			#endif
		}
		
	}
}

void Game::update()
{
	// update
	if(_needUpdate && !_pauseUpdate)
	{
		snake.update(_gameFieldH, _gameFieldW, food, *this); // TODO: FUCKING BUG
	}
	else
		_pauseUpdate = false;
	// judge
}
