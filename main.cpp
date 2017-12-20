#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 5

#define DEBUG 1

#if DEBUG
	#define PRE_SET_GENERATE_POS 0
	#define PRE_SET_SNAKE_DIR 0
	
	#if PRE_SET_GENERATE_POS
		#define SNAKE_X WIDTH - 1
		#define SNAKE_Y 5
		
		#define FOOD_X WIDTH - 1
		#define FOOD_Y 0
	#endif
	#if PRE_SET_SNAKE_DIR
		#define SNAKE_DIR 0
	#endif
	
	#define DEBUG_O WIDTH + 2
	int debug_line = 0;
	#define OUTPUT 0
#endif

/* Thinkings:
 * 1. How to make the timer?
 *     (1) refresh the screen per 0.5s - DONE
 *     (2) use permanent fps timer and specifies the speed of snake -TODO
 * 2. Data Structure of snake
 *     (1) Realize it by my own -DONE
 *     (2) Use Vector<>         -TODO
 * 
 * BUGS:
 *     (1) When the snake is near the border (1 blcok), sometimes if you turn left or right (on snake's viewpoint)
 *         the snake's head or its body will disappear one block. -DONE
 *     (2) If one press the other two arrow keys except snake's direction and the opposite arrow key (up <-> down)
 *         and switch to the opposite key to the snake's direction quickly, the snake will be able to move to worng way. 
 */
 


void arrayPos(int x, int y)
{
    COORD cursor;
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    cursor.X = x;
    cursor.Y = y;
    SetConsoleCursorPosition(out, cursor);
}

void HideConsoleCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.dwSize = 1;//cursor size
    cursorInfo.bVisible = false;//cursor visible
    SetConsoleCursorInfo(out, &cursorInfo);
}

class Game
{
	public:
		Game();
		~Game();
		void start();
		bool generateGamePlace(const int x, const int y);
	private:
		void update();
		//void judge();
		void processInput();
		int getInput();
		
		void printMap();
		
		class Timer
		{
			public:
				float start, end, elapsed;
				void timeStart();
				void timeEnd();
				Timer() : start(0.f), end(0.f), elapsed(0.f) 
				{} 
		}timer;
		
		class Snake;
		struct Food;
		class Screen
		{
			public:
				Screen() : snakeBody(0)
				{}
				void setBufferSize(const int screenh, const int screenw);
				void drawGameField(Game &cGame, char **game, const int gameFieldH, const int gameFieldW, Snake &snake, Food &food);
				void flipScreen();
			private:
				char **_buffer;
				int screenH, screenW;
				int snakeBody;
		}screen;
		
		enum Snake_Direction
		{
			DIR_NONE = -1,
			DIR_UP,
			DIR_DOWN,
			DIR_LEFT,
			DIR_RIGHT
		};
		enum Wall_Option
		{
			WALL_CLOSE,
			WALL_OPEN
		};
		enum Arrow_Key_code
		{
			ARR_UP    = 72,
			ARR_DOWN  = 80,
			ARR_LEFT  = 75,
			ARR_RIGHT = 77
		};
		enum Key_handle
		{
			KEY_NOTPRESS,
			KEY_NOTHANDLE,
			KEY_ESC,
			KEY_UP,
			KEY_DOWN,
			KEY_LEFT,
			KEY_RIGHT
		};
		enum Judge_Condition
		{
			JUDGE_EAT = 1,
			JUDGE_NOTEAT,
			JUDGE_DEAD
		};
		/* TODO (roy4801#1#): get some functions into private. */
		class Snake
		{
			public:
				struct Snake_Body;
				void generate(const int gameFieldH, const int gameFieldW);
				void update(const int h, const int w, Food &food, Game &game);
				void eat(const int x, const int y) { addSnakeHead(x, y); length++; }
				int judge(const int h, const int w, Food &food, Game &game, const int ux, const int uy);//add 2.38
				void dead() { isDead = true; }
				bool getDead() { return isDead; }
				void stopMove() { setDirection(DIR_NONE); }

				void setDirection(int dir) { direction = dir; }
				int getDirection() { return direction; }
				Snake_Body* getHead() { return b_head; }
			//private:
				void addSnakeBody(const int posx, const int posy);
				void addSnakeHead(const int posx, const int posy);
				void pop_back();
			
				struct Snake_Body
				{
					Snake_Body() : x(-1), y(-1), next(NULL)
					{}
					~Snake_Body()
					{
						delete next;
					}
					int x, y;
					Snake_Body *next;
				};
				
				Snake_Body *b_head, *b_current;
				char symbol;
				int length;
				int direction;
				Snake() : symbol('@'), length(5), direction(DIR_NONE), b_head(NULL), b_current(NULL), isDead(false)
				{
					b_head = b_current = new Snake_Body;
				}
			private:
				bool isDead;
		}snake;
		class Food
		{
			public:
			void generate(const int gameFieldH, const int gameFieldW, Snake &snake);
			char symbol;
			int x, y;
			Food() : symbol('*'), x(-1), y(-1)
			{}
		}food;
		
		char **_gameField;
	private:
		int _gameFieldW, _gameFieldH;
		bool _running;
		
		bool _needUpdate;
		bool _pauseUpdate;
		bool _blockArrowKey;
		int _wallType;
	public:
		int getWallType() { return _wallType; }
		void setWallType(int type) { _wallType = type; } 
		bool getPauseUpdate() { return _pauseUpdate; }
		void setPauseUpdate(bool p) { _pauseUpdate = p; }
		void setBlockArrowKey(bool k) { _blockArrowKey = k; }
		bool getBlockArrowKey() { return _blockArrowKey; }
		int getGameFieldH() { return _gameFieldH; }
		int getGameFieldW() { return _gameFieldW; }
			
};

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

// Game::Snake
void Game::Snake::generate(const int gameFieldH, const int gameFieldW)
{
	#if PRE_SET_GENERATE_POS
	int x = SNAKE_X;
	int y = SNAKE_Y;
	#else
	int x = rand() % gameFieldW;
	int y = rand() % gameFieldH;
	#endif
	
	int nowLength = 1;
	addSnakeBody(x, y); // head
	
	int bodyLength = length - 1;
	
	while(nowLength < bodyLength)
	{
		#if PRE_SET_SNAKE_DIR
		int dir = SNAKE_DIR;
		#else
		int dir = rand() % 4;
		#endif
		switch(dir)// Grow way
		{
			case DIR_UP: // head down
				if(y <= gameFieldH - 3)
				{
					for(int i = 0; i < bodyLength; i++)
					{
						addSnakeBody(x, ++y);
						nowLength++;
					}
				}
				break;
			case DIR_DOWN: // head up
				if(y >= 2)
				{
					for(int i = 0; i < bodyLength; i++)
					{
						addSnakeBody(x, --y);
						nowLength++;
					}
				}
				break;
			case DIR_LEFT: // head right
				if(x >= 2)
				{
					for(int i = 0; i < bodyLength; i++)
					{
						addSnakeBody(--x, y);
						nowLength++;
					}
				}
				break;
			case DIR_RIGHT: // head left
				if(x <= gameFieldW - 3)
				{
					for(int i = 0; i < bodyLength; i++)
					{
						addSnakeBody(++x, y);
						nowLength++;
					}
				}
				break;
		}
		if(nowLength == length) // Set head dir
		{
			switch(dir)//grow way
			{
				case DIR_UP:
					setDirection(DIR_DOWN);
					break;
				case DIR_DOWN:
					setDirection(DIR_UP);
					break;
				case DIR_LEFT:
					setDirection(DIR_RIGHT);
					break;
				case DIR_RIGHT:
					setDirection(DIR_LEFT);
					break;
			}
		}
	}

} 
void Game::Snake::update(const int h, const int w, Food &food, Game &game) //TODO : remove bool return 
{
	int updateX = b_head->x,
		updateY = b_head->y;
	if(getDead())
	{
		setDirection(DIR_NONE);
	}
	/* Snake move and judgement*/
	if(direction != DIR_NONE)
	{
		switch(direction)
		{
			case DIR_UP:
				updateY++;
				break;
			case DIR_DOWN:
				updateY--;
				break;
			case DIR_LEFT:
				updateX--;
				break;
			case DIR_RIGHT:
				updateX++;
				break;
		}
		
		int result = judge(h, w, food, game, updateX, updateY); //TODO: shits happened
		
		if(result == JUDGE_NOTEAT)
		{
			/* The snake move one block */
			addSnakeHead(updateX, updateY);
			pop_back();
//			return false;
		}
		if(result == JUDGE_EAT)
		{
			//pop_back(); // because the head has appened a new head (food.x, food.y), so it doesn't need to pop_back()
//			return true;
		}
	}
}

int Game::Snake::judge(const int h, const int w, Food &food, Game &game, const int ux, const int uy)
{
	Snake_Body *head = getHead();
	// Touch wall judge
	switch(game.getWallType())
	{
		case WALL_CLOSE:
		{
			if(head->x > w - 1 || head->y > h - 1 || head->x < 0 || head->y < 0) // on the border
			{
				// warning
				#if DEBUG
				arrayPos(DEBUG_O, OUTPUT + 3 + debug_line++);
				printf("%s", "Snake is dead");
				#endif
				dead();
				setDirection(DIR_NONE);
			}
		}
		break;
		case WALL_OPEN:
			
		break;
	}
	
	bool eaten = false;
	
	// Snake eats the food
	/*switch(direction)
	{
		case DIR_UP:
			if(ux == food.x && uy + 1 == food.y)
			{
				
				eaten = true;
			}
			break;
		case DIR_DOWN:
			if(ux == food.x && uy - 1 == food.y)
			{
				
				eaten = true;
			}
			break;
		case DIR_LEFT:
			if(ux - 1 == food.x && uy == food.y)
			{
				
				eaten = true;
			}
			break;
		case DIR_RIGHT:
			if(ux + 1 == food.x && uy == food.y)
			{

				eaten = true;
			}
			break;
	}*/
	if(ux == food.x && uy == food.y)
	{
		eaten = true;
	}

	if(eaten)
	{
//		game.setPauseUpdate(true); //
		eat(food.x, food.y);
		food.generate(h, w, *this);
		return JUDGE_EAT;
	}
	else
	{
//		game.setPauseUpdate(false);
		return JUDGE_NOTEAT;
	}
}

void Game::Snake::addSnakeBody(const int posx, const int posy)//9.45
{
	b_current->x = posx;
	b_current->y = posy;
	
	b_current->next = new Snake_Body;
	b_current = b_current->next;
}

void Game::Snake::addSnakeHead(const int posx, const int posy)
{
	if(posx == b_head->x && posy == b_head->y)
		assert(0);
	Snake_Body *newHead = new Snake_Body;
		
	newHead->x = posx;
	newHead->y = posy;
		
	newHead->next = b_head;
	b_head = newHead;
}

void Game::Snake::pop_back()
{
	for(Snake_Body *i = b_head;
		i->next != NULL;
		i = i->next)
	{
		if(i->next->next->next == NULL)
		{
			Snake_Body *nextLeft = i->next->next;	// keep next of want to delete
			i->next->next = NULL;					// prevent delete all of them
			delete i->next;

			i->next = nextLeft;
		}
	}
}

//Game::Food::
void Game::Food::generate(const int gameFieldH, const int gameFieldW, Snake &snake) // TODO: position of food may show up in the same pos of snake bodies
{
	/* x, y in this section means Food.x, Food.y */
	#if PRE_SET_GENERATE_POS
	x = FOOD_X;
	y = FOOD_Y;
	#else
	while(0) //DEBUG 2017-11-22
	{
		//if(food.x != -1 && food.y != -1)
		int newFoodX = rand() % gameFieldW;
		int newFoodY = rand() % gameFieldH; // 0~9
		int check = 0;
		
		for(Snake::Snake_Body *s = snake.getHead();
			s->next != NULL;
			s = s->next)
		{
			if(newFoodX != s->x && newFoodY != s->y)
			{
				check++;
			}
		}
		// if the new food doesn't have as same pos as old food && doesn't overlap with the snake
		if(check == snake.length && x != newFoodX && y != newFoodY)
		{
			x = newFoodX;
			y = newFoodY;
			//_gameField[_gameFieldH - food.x][food.y] = food.symbol;
			break;
		}
	}
	// Place on corner
	int pos = rand() % 4;
	switch(pos)
	{
		case 0:
			x = y = 0;
			break;
		case 1:
			x = gameFieldW - 1;
			y = gameFieldH - 1;
			break;
		case 2:
			x = 0;
			y = gameFieldH - 1;
			break;
		case 3:
			x = gameFieldW - 1;
			y = 0;
			break;
	}
	
	#endif
} 
int main()
{
	HideConsoleCursor();
	srand(time(NULL));
	Game game;
	game.generateGamePlace(HEIGHT, WIDTH);
	game.start();
	return 0;
}
