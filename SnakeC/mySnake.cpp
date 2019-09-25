#include "stdio.h"
#include "conio.h"
#include "windows.h"

const int WIDTH = 40; // ширина
const int HEIGHT = 20; // высота
const int SPEED = 300; // скорость (пауза перед сменой кадров в миллисекундах)
int gameOver = 0;
int xPos, yPos; // координаты головы
int xPrizePos, yPrizePos; // координаты приза
int score = 0; // счет (счет/10 == размер хвоста)
int tailX[100] = { 0 }, tailY[100] = { 0 }; // массивы с координатами для хвоста
int flag;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };  // направление движения
enum eDirection dir;

HANDLE hStdOut;

void gotoxy(int x, int y) // установка курсора на координаты
{
	COORD c = { x, y };
	SetConsoleCursorPosition(hStdOut, c);
}

void setup() // первичные настройки игры
{
	for (int i = 0; i < HEIGHT; i++) { // рисовка границы
		for (int j = 0; j < WIDTH; j++) {
			if (j == 0 || j == WIDTH - 1 || i == 0 || i == HEIGHT - 1) printf("#");
			else printf(" ");
		}
		printf("\n");
	}

	xPos = WIDTH / 2; // установка начальных координат головы
	yPos = HEIGHT / 2;

	do // установка начальных координат приза
	{
		xPrizePos = 1 + rand() % (WIDTH - 2);
		yPrizePos = 1 + rand() % (HEIGHT - 2);
	} while (xPos == xPrizePos && yPos == yPrizePos); // выбираем новые координаты, если они совпадают с головой
}

void drawHead() // рисовка головы
{
	gotoxy(xPos, yPos);
	printf("O");
}

void deleteHead() // стирание головы
{
	gotoxy(xPos, yPos);
	printf(" ");
}

void drawTail() // рисовка хвоста
{
	if (score > 0) {
		for (int i = 0; tailX[i] != 0; i++)
		{
			gotoxy(tailX[i], tailY[i]);
			printf("o");
		}
	}
}

void deleteTail() // удаление последнего элемента хвоста
{
	if (score > 0) {
		gotoxy(tailX[score / 10 - 1], tailY[score / 10 - 1]);
		if (tailX[score / 10 - 1] != 0) printf(" "); // проверка, чтобы не удалять символ в координате 0.0
	}
}

void drawPrize() // рисовка приза
{
	gotoxy(xPrizePos, yPrizePos);
	printf("+");
}


void input() // считывание ввода
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

void logic() // логика игры
{
	if (score > 0) { // сдвиг координат хвоста перед новой прорисовкой
		for (int i = score / 10 - 1; i > 0; i--) {
			tailX[i] = tailX[i - 1];
			tailY[i] = tailY[i - 1];
		}
	}

	tailX[0] = xPos; // новые координаты первого элемента хвоста
	tailY[0] = yPos;

	switch (dir) // изменение позиции головы на основе введенной клавиши
	{
	case LEFT: xPos--;  break;
	case RIGHT: xPos++; break;
	case UP: yPos--; break;
	case DOWN: yPos++; break;
	}

	if (xPos == xPrizePos && yPos == yPrizePos) // поиск позиции нового приза при поедание
	{
		flag = 0;
		while (flag == 0) {
			xPrizePos = 1 + rand() % (WIDTH - 2);
			yPrizePos = 1 + rand() % (HEIGHT - 2);
			for (int i = 0; i < 100; i++)
			{
				if ((xPrizePos == tailX[i] && yPrizePos == tailY[i]) || (xPrizePos == xPos && yPrizePos == yPos)) { // если совпадает хвост со змеей
					flag = 0; // оставляем флаг нулевым, чтобы задать новые координаты приза
					break; // и заканчиваем выполнение цикла
				}
				else flag = 1; // иначе закончим цикл с поднятым флагом и выходим из верхнего цикла
			}
		}
		score += 10; // увеличиваем счет на 10
	}

	if (xPos == WIDTH - 1 || yPos == HEIGHT - 1 || xPos == 0 || yPos == 0) gameOver = 1; // останавливает игру, если координаты головы совпали со стеной

	for (int i = 0; i < 100; i++) { // останавливает игру, если координаты головы совпали с хвостом
		if (score > 0 && xPos == tailX[i] && yPos == tailY[i]) gameOver = 1;
	}
}

void drawScore() // выводим счет
{
	gotoxy(0, HEIGHT + 1);
	printf("SCORE: %3.d", score);
}

void drawFinale() // выводим финальный счет
{
	gotoxy(0, HEIGHT + 1);
	printf("                 ");

	gotoxy(0, 0);
	for (int i = 0; i < HEIGHT; i++) { // рисовка границы
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

	setup(); // первичные настройки (игровое поле, положение головы и приза)

	while (!gameOver) {
		drawPrize(); // рисуем приз
		drawTail(); // рисуем хвост
		drawHead(); // рисуем голову
		gotoxy(WIDTH, HEIGHT); // переводим курсора в нижний правый край, чтобы не мешал при игре
		Sleep(SPEED); // пауза
		deleteHead(); // удаляем голову
		deleteTail(); // удаляем последний элемент хвоста
		input(); // считываем ввод с клавиатуры
		logic(); // применяем логику (выставляем новые координаты и значения переменных в зависимости от событий в игре)
		drawScore(); // выводим счет
	}

	drawFinale(); // выводим сообщение об окончание игры с результат
}