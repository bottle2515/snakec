#include "snake_game.h"
#include "defines.h"

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
