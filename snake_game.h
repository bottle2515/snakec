#ifndef _SNAKE_GAME_H_
#define _SNAKE_GAME_H_

// #include <stdio.h>
#include <iostream>
#include <vector>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>

#include "defines.h"
#include "utils.h"

using std::string;
using std::vector;

class Game
{
	public:
		Game();
		~Game();
		void start();
		bool generateGamePlace(const int x, const int y);
	private:
		void update();
		void processInput();
		int getInput();
		
		void printMap();
		
		class Timer
		{
			public:
				class Timecell
				{
					public:
						float start, end, elapsed;
						void timeStart();
						void timeEnd();
						float getElapsed();
						Timecell() : start(0.f), end(0.f), elapsed(0.f) 
						{}
				};

				enum Timer_Type
				{
					TIMER_COUNTDOWN,
					TIMER_COUNTUP
				};
				// Functions
				Timer();
				// ~Timer();
				void addTimer(const char timerName[], const int type);
				float getNowTime(const char timerName[]);
			private:
				vector<Timecell> timerList;
				vector<string> timerName;
				float nowTime;
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

#endif
