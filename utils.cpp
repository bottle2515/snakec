#include "utils.h"

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
    cursorInfo.dwSize = 1;			//cursor size
    cursorInfo.bVisible = false;	//cursor visible
    SetConsoleCursorInfo(out, &cursorInfo);
}

