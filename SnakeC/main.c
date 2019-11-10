#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define SPEED 200

HANDLE hStdOut;

enum colors_enum
{
	Black = 0,
	Grey = FOREGROUND_INTENSITY,
	LightGrey = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	Blue = FOREGROUND_BLUE,
	Green = FOREGROUND_GREEN,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	Red = FOREGROUND_RED,
	Purple = FOREGROUND_RED | FOREGROUND_BLUE,
	LightBlue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	LightGreen = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	LightCyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	LightRed = FOREGROUND_RED | FOREGROUND_INTENSITY,
	LightPurple = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	Orange = FOREGROUND_RED | FOREGROUND_GREEN,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
};

int gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(hStdOut, c);
}

void setcolor(WORD attr)
{
	SetConsoleTextAttribute(hStdOut, attr);
}

void Setup()
{
	gameOver = 0;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void Draw()
{
	system("cls");
	for (int i = 0; i < width + 1; i++)	printf("#");
	printf("\n");

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0 || j == width - 1) printf("#");
			if (i == y && j == x) printf("0");
			else if (i == fruitY && j == fruitX) printf("F");
			else
			{
				int print = 0;
				for (int k = 0; k < nTail; k++)
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						print = 1;
						printf("o");
					}
				}
				if (!print) printf(" ");
			}
		}
		printf("\n");
	}

	for (int i = 0; i < width + 1; i++) printf("#");
	printf("\n");
	printf("Score: %d\n",score);
	if(score<SPEED) Sleep(SPEED-score);
	else Sleep(SPEED - (SPEED - 10));
}

void Input()
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			if (dir != RIGHT || nTail == 0)dir = LEFT; break;
		case 'd':
			if (dir != LEFT || nTail == 0) dir = RIGHT; break;
		case 'w':
			if (dir != DOWN || nTail == 0) dir = UP; break;
		case 's':
			if (dir != UP || nTail == 0) dir = DOWN; break;
		case 'q':
			dir = STOP; break;
		case 'x':
			gameOver = 1; break;
		}
	}
}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir)
	{
	case LEFT: x--;  break;
	case RIGHT: x++; break;
	case UP: y--; break;
	case DOWN: y++; break;
	}

	//if (x > width || x < 0 || y > height || y < 0)		gameOver = true;
	if (x >= width - 1)		x = 0;
	else if (x < 0)		x = width - 2;

	if (y >= height)		y = 0;
	else if (y < 0)		y = height - 1;

	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)		gameOver = 1;
	}

	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}

}

int main()
{
	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
	}

	return 0;
}
