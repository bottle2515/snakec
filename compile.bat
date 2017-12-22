@echo off

cls

echo Snake Compiling... Written by roy4801

set EXE_NAME=snake.exe
set /A flag_cp=0

rem if not exist %OBJ_DIR%/ (
rem 	mkdir %OBJ_DIR%
rem )

if exist *.o (
	echo Cleaning last object files...
	del /q *.o
)


set SRC_FILE=snake_game.o game_snake.o game_food.o game_screen.o main.o defines.o utils.o game_timer.o

::Compile
echo Compiling...
g++ -c snake_game.cpp -o snake_game.o
g++ -c game_timer.cpp -o game_timer.o
g++ -c game_snake.cpp -o game_snake.o
g++ -c game_food.cpp -o game_food.o
g++ -c game_screen.cpp -o game_screen.o
g++ -c main.cpp -o main.o
g++ -c defines.cpp -o defines.o
g++ -c utils.cpp -o utils.o

if %ERRORLEVEL% EQU 0 (
	::Linking
	echo Linking...
	g++ %SRC_FILE% -o %EXE_NAME%
	del /q *.o
)

if %ERRORLEVEL% EQU 0 (
	echo Build Complete!
)

rem if %flag_cp% EQU 1 (
rem )