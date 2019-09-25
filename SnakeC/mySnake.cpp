#include "stdio.h"
#include "conio.h"
#include "windows.h"

const int WIDTH = 40; // ������
const int HEIGHT = 20; // ������
const int SPEED = 300; // �������� (����� ����� ������ ������ � �������������)
int gameOver = 0;
int xPos, yPos; // ���������� ������
int xPrizePos, yPrizePos; // ���������� �����
int score = 0; // ���� (����/10 == ������ ������)
int tailX[100] = { 0 }, tailY[100] = { 0 }; // ������� � ������������ ��� ������
int flag;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };  // ����������� ��������
enum eDirection dir;

HANDLE hStdOut;

void gotoxy(int x, int y) // ��������� ������� �� ����������
{
	COORD c = { x, y };
	SetConsoleCursorPosition(hStdOut, c);
}

void setup() // ��������� ��������� ����
{
	for (int i = 0; i < HEIGHT; i++) { // ������� �������
		for (int j = 0; j < WIDTH; j++) {
			if (j == 0 || j == WIDTH - 1 || i == 0 || i == HEIGHT - 1) printf("#");
			else printf(" ");
		}
		printf("\n");
	}

	xPos = WIDTH / 2; // ��������� ��������� ��������� ������
	yPos = HEIGHT / 2;

	do // ��������� ��������� ��������� �����
	{
		xPrizePos = 1 + rand() % (WIDTH - 2);
		yPrizePos = 1 + rand() % (HEIGHT - 2);
	} while (xPos == xPrizePos && yPos == yPrizePos); // �������� ����� ����������, ���� ��� ��������� � �������
}

void drawHead() // ������� ������
{
	gotoxy(xPos, yPos);
	printf("O");
}

void deleteHead() // �������� ������
{
	gotoxy(xPos, yPos);
	printf(" ");
}

void drawTail() // ������� ������
{
	if (score > 0) {
		for (int i = 0; tailX[i] != 0; i++)
		{
			gotoxy(tailX[i], tailY[i]);
			printf("o");
		}
	}
}

void deleteTail() // �������� ���������� �������� ������
{
	if (score > 0) {
		gotoxy(tailX[score / 10 - 1], tailY[score / 10 - 1]);
		if (tailX[score / 10 - 1] != 0) printf(" "); // ��������, ����� �� ������� ������ � ���������� 0.0
	}
}

void drawPrize() // ������� �����
{
	gotoxy(xPrizePos, yPrizePos);
	printf("+");
}


void input() // ���������� �����
{
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			if (dir != RIGHT || score == 0) dir = LEFT; break;
		case 'd':
			if (dir != LEFT || score == 0) dir = RIGHT; break;
		case 'w':
			if (dir != DOWN || score == 0) dir = UP; break;
		case 's':
			if (dir != UP || score == 0) dir = DOWN; break;
		}
	}
}

void logic() // ������ ����
{
	if (score > 0) { // ����� ��������� ������ ����� ����� �����������
		for (int i = score / 10 - 1; i > 0; i--) {
			tailX[i] = tailX[i - 1];
			tailY[i] = tailY[i - 1];
		}
	}

	tailX[0] = xPos; // ����� ���������� ������� �������� ������
	tailY[0] = yPos;

	switch (dir) // ��������� ������� ������ �� ������ ��������� �������
	{
	case LEFT: xPos--;  break;
	case RIGHT: xPos++; break;
	case UP: yPos--; break;
	case DOWN: yPos++; break;
	}

	if (xPos == xPrizePos && yPos == yPrizePos) // ����� ������� ������ ����� ��� ��������
	{
		flag = 0;
		while (flag == 0) {
			xPrizePos = 1 + rand() % (WIDTH - 2);
			yPrizePos = 1 + rand() % (HEIGHT - 2);
			for (int i = 0; i < 100; i++)
			{
				if ((xPrizePos == tailX[i] && yPrizePos == tailY[i]) || (xPrizePos == xPos && yPrizePos == yPos)) { // ���� ��������� ����� �� �����
					flag = 0; // ��������� ���� �������, ����� ������ ����� ���������� �����
					break; // � ����������� ���������� �����
				}
				else flag = 1; // ����� �������� ���� � �������� ������ � ������� �� �������� �����
			}
		}
		score += 10; // ����������� ���� �� 10
	}

	if (xPos == WIDTH - 1 || yPos == HEIGHT - 1 || xPos == 0 || yPos == 0) gameOver = 1; // ������������� ����, ���� ���������� ������ ������� �� ������

	for (int i = 0; i < 100; i++) { // ������������� ����, ���� ���������� ������ ������� � �������
		if (score > 0 && xPos == tailX[i] && yPos == tailY[i]) gameOver = 1;
	}
}

void drawScore() // ������� ����
{
	gotoxy(0, HEIGHT + 1);
	printf("SCORE: %3.d", score);
}

void drawFinale() // ������� ��������� ����
{
	gotoxy(0, HEIGHT + 1);
	printf("                 ");

	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) { // ������� �������
		for (int j = 0; j < WIDTH; j++) {
			if (j == 0 || j == WIDTH - 1 || i == 0 || i == HEIGHT - 1) printf("#");
			else printf(" ");
		}
		printf("\n");
	}

	gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 4);
	printf("GAME OVER!!!");
	gotoxy(WIDTH / 2 - 10, HEIGHT / 2 - 2);
	printf("FINAL SCORE: %d\n", score);
	gotoxy(WIDTH / 2 - 10, HEIGHT / 2);
	printf("press any key to exit");
	gotoxy(0, HEIGHT);
}

void main()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	srand(time(NULL));

	setup(); // ��������� ��������� (������� ����, ��������� ������ � �����)

	while (!gameOver) {
		drawPrize(); // ������ ����
		drawTail(); // ������ �����
		drawHead(); // ������ ������
		gotoxy(WIDTH, HEIGHT); // ��������� ������� � ������ ������ ����, ����� �� ����� ��� ����
		Sleep(SPEED); // �����
		deleteHead(); // ������� ������
		deleteTail(); // ������� ��������� ������� ������
		input(); // ��������� ���� � ����������
		logic(); // ��������� ������ (���������� ����� ���������� � �������� ���������� � ����������� �� ������� � ����)
		drawScore(); // ������� ����
	}

	drawFinale(); // ������� ��������� �� ��������� ���� � ���������
}