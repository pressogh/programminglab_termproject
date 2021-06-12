#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h> //윈도우즈와 관련된 것을 처리해주는 입출력 함수입니다.
#include <ctype.h>
#include <mmsystem.h>
#include "term_project.h"
#include <math.h>

#include <stdbool.h>

#pragma comment(lib, "winmm.lib")

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void) { // 커서를 보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void cls(int text_color, int bg_color) // 화면 지우기
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
}

void draw_box(int x1, int y1, int x2, int y2, char ch)
{
	int x, y;
	gotoxy(x1, y1);
	for (int i = y1; i < y2; i++)
	{
		for (int j = x1; j < x2; j++)
		{
			if (i == y1 || i == y2 - 1) printf("%c", ch);
			else
			{

				if (j == x1 || j == x2 - 1) printf("%c", ch);
				else printf(" ");
			}
		}
		printf("\n");
	}
}

void draw_box2(int x1, int y1, int x2, int y2, char* ch)
{
	int x, y;
	gotoxy(x1, y1);
	for (int i = y1; i < y2; i++)
	{
		for (int j = x1; j < x2; j++)
		{
			if (i == y1 || i == y2 - 1)
			{
				gotoxy(j, i);
				printf("%s", ch);
			}
			else
			{

				if (j == x1 || j == x2 - 1)
				{
					gotoxy(j, i);
					printf("%s", ch);
				}
			}
		}
		printf("\n");
	}
}

//
// 맵 관련
//

// 저장된 맵
int stage_saved[STAGE_COUNT][STAGE_WIDTH][STAGE_HEIGHT] = 
{
	{
		1, 1, 2, 2, 3, 3, 4, 4,
		1, 1, 2, 2, 3, 3, 4, 0,
		3, 3, 4, 4, 1, 1, 2, 2,
		3, 3, 4, 4, 1, 1, 2, 0,
		1, 1, 6, 6, 5, 5, 4, 4,
		1, 1, 6, 6, 5, 5, 4, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{
		0, 0, 0, 7, 7, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 0,
		0, 0, 0, 3, 0, 0, 0, 0,
		0, 0, 0, 4, 0, 0, 0, 0,
		0, 0, 0, 5, 0, 0, 0, 0,
		0, 0, 0, 6, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	},
	{
		1, 1, 2, 2, 3, 3, 4, 4,
		1, 1, 2, 2, 3, 3, 4, 0,
		3, 3, 4, 4, 1, 1, 2, 2,
		3, 3, 4, 4, 1, 1, 2, 0,
		1, 1, 6, 6, 5, 5, 4, 4,
		1, 1, 6, 6, 5, 5, 4, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	}
};

int stage;
bool game_active = true;
int stage_now[STAGE_HEIGHT][STAGE_WIDTH] = {
	{ 1, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 2, 2, 3, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 4, 2, 1, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0 }
};
int ball_now = 1, ball_next = 2;


typedef struct _Launcher
{
	int x;
	int y;
	float angle;
	int size;
}_Launcher;

void init()
{
	system("mode con cols=150 lines=50");
	srand(time(NULL));
	removeCursor();
}

void startScreen(int n)
{
	// 백그라운드 색상
	printf("\033[%s;2;%d;%d;%dm", "48", 0, 14, 46);

	//draw stars
	for (int i = 0; i < 50; i++)
	{
		gotoxy(rand() % 150, rand() % 50);
		
		int r = rand() % 255;
		int g = rand() % 255;
		int b = rand() % 255;
		
		printf("\033[%s;2;%d;%d;%dm", "38", r, g, b);
		printf("*");
	}

	printf("\033[%s;2;%d;%d;%dm", "38", 246, 246, 6);
	draw_box2(34, 10, 111, 30, "■");

	printf("\033[%s;2;%d;%d;%dm", "38", 255, 255, 255);
	gotoxy(65, 15);
	printf("Puzzle");
	gotoxy(75, 20);
	printf("Bobble");

	if (n == 1)
	{
		gotoxy(65, 35);
		printf("PUSH ENTER TO START");
	}

	gotoxy(50, 20);
	printf("\033[%s;2;%d;%d;%dm", "38", 255, 255, 255);
	
	
	gotoxy(0, 49);
}

static HDC hdc, MemDC, tmpDC;
static PAINTSTRUCT ps;
static HBITMAP BackBit, oldBackBit;
static RECT bufferRT;
static HWND hwnd;

void startBuffer()
{
	hwnd = GetForegroundWindow();
	hdc = GetDC(hwnd);

	SetGraphicsMode(hdc, GM_ADVANCED);
	
	GetClientRect(hwnd, &bufferRT);
	MemDC = CreateCompatibleDC(hdc);
	BackBit = CreateCompatibleBitmap(hdc, bufferRT.right, bufferRT.bottom);
	oldBackBit = (HBITMAP)SelectObject(MemDC, BackBit);
	PatBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);
	tmpDC = hdc;
	hdc = MemDC;
	MemDC = tmpDC;
}

void endBuffer()
{
	tmpDC = hdc;
	hdc = MemDC;
	MemDC = tmpDC;
	GetClientRect(hwnd, &bufferRT);
	BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(MemDC);
	ReleaseDC(hwnd, hdc);
	EndPaint(hwnd, &ps);
}

void drawWall()
{
	int x = 350, y = 50;
	int i;
	for (i = 0; i < STAGE_HEIGHT; i++)
	{
		Rectangle(hdc, x, y + i * WALL_SIZE, x + WALL_SIZE, y + (i + 1) * WALL_SIZE);
	}
	
	for (i = 0; i < 10; i++)
	{
		Rectangle(hdc, x + i * WALL_SIZE, y, x + (i + 1) * WALL_SIZE, y + WALL_SIZE);
	}

	x = x + (i - 1) * WALL_SIZE;
	
	for (i = 0; i < STAGE_HEIGHT; i++)
	{
		Rectangle(hdc, x, y + i * WALL_SIZE, x + WALL_SIZE, y + (i + 1) * WALL_SIZE);
	}
}

void drawOneBall(int color,	float x, float y, int r)
{
	int tmpCol;
	if (color == 1) tmpCol = RGB(255, 99, 99);
	else if (color == 2) tmpCol = RGB(99, 255, 99);
	else if (color == 3) tmpCol = RGB(99, 99, 255);
	else if (color == 4) tmpCol = RGB(255, 255, 51);
	
	HBRUSH hbrush = CreateSolidBrush(tmpCol);
	HGDIOBJ h_old_brush = SelectObject(hdc, hbrush);
	
	Ellipse(hdc, x, y, x + r, y + r);
	
	SelectObject(hdc, h_old_brush);
	DeleteObject(hbrush);
}

void drawMap()
{
	int idx = 0;
	float g_fBallSpace = 0.87f;

	// 공이 그려질 위치
	float fDrawX = 0.0f, fDrawY = 0.0f;

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		fDrawY = WALL_SIZE + BALL_SIZE * g_fBallSpace * i;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			idx = stage_now[i][j];
			if (idx == 0) continue;
			
			if (i % 2) fDrawX = (float)(WALL_SIZE + BALL_SIZE / 2.0f + BALL_SIZE * j);
			else fDrawX = (float)(WALL_SIZE + j * BALL_SIZE);
			drawOneBall(idx, fDrawX + 350, fDrawY + 50, BALL_SIZE);
		}
	}
}

void drawLauncher(_Launcher L)
{	
	float r = L.angle * 3.141592f / 180.0f;
	float c = cos(r), s = sin(r);

	drawOneBall(ball_next, 500, 550, BALL_SIZE);


	MoveToEx(hdc, L.x, L.y, NULL);
	LineTo(hdc, L.x + L.size * c, L.y + L.size * s);
	
	drawOneBall(ball_now, 575, 550, BALL_SIZE);	
}

void selectNewBall()
{
	int arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (stage_now[i][j] != 0) arr[stage_now[i][j]]++;
		}
	}
	
	while(1)
	{
		int tmp = 1 + rand() % 7;
		ball_next = tmp;
		if (arr[tmp] != 0) break;
	}
}

void inGame()
{
	system("cls");
	PlaySound(TEXT("Puzzle Bobble.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
	
	_Launcher L;
	L.x = 600;
	L.y = 575;
	L.angle = -90.0f;
	L.size = 50;
	
	while(game_active)
	{
		startBuffer();


		drawWall();
		drawMap();
		drawLauncher(L);
		
		if (_kbhit())
		{
			int tmp;
			tmp = _getch();
			
			if (tmp == 32) break;
			else if (tmp == 224)
			{
				int ch = _getch();
				if (ch == 75) L.angle -= 5;
				else if (ch == 77) L.angle += 5;
				
				if (L.angle <= -185.0f) {
					L.angle = -180.0f;
					continue;
				}
				else if (L.angle >= 5.0f)
				{
					L.angle = 0.0f;
					continue;
				}
				
			}
		}
		
		endBuffer();
	}

}

int main()
{
	init();

	while(1)
	{
		if (_kbhit())
		{
			if (_getch() == 13) break;
		}
		startScreen(0);
		Sleep(500);
		system("cls");
		startScreen(1);
		Sleep(500);
		system("cls");
	}

	inGame();

	return 0;
}