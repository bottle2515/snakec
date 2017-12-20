#include "snake_game.h"
#include "utils.h"

//Game::Snake::
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