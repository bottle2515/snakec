/* Thinkings:
 * 1. How to make the timer?
 *     (1) refresh the screen per 0.5s - DONE
 *     (2) use permanent fps timer and specifies the speed of snake -DOING
 * 2. Data Structure of snake
 *     (1) Realize it by my own -DONE
 *     (2) Use Vector<>         -TODO
 * 3. Snake judgement for touching its body by itself.
 * 
 * BUGS:
 *     (1) When the snake is near the border (1 blcok), sometimes if you turn left or right (on snake's viewpoint)
 *         the snake's head or its body will disappear one block. -DONE
 *     (2) If one press the other two arrow keys except snake's direction and the opposite arrow key (up <-> down)
 *         and switch to the opposite key to the snake's direction quickly, the snake will be able to move to worng way. 
 */