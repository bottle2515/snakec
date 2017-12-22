#ifndef _DEFINES_H_
#define _DEFINES_H_

#define WIDTH 20
#define HEIGHT 15

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
	extern int debug_line;
	#define OUTPUT 0
#endif

#endif
