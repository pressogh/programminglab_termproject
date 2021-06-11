#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h> //윈도우즈와 관련된 것을 처리해주는 입출력 함수입니다.
#include <ctype.h>
#include <mmsystem.h>
#include "term_project.h"

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
int stage_saved[STAGE_COUNT][STAGE_WIDTH * STAGE_HEIGHT] = 
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
int stage_now[STAGE_WIDTH][STAGE_HEIGHT] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

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
	int r = 50;
	int x = 350, y = 50;
	int i;
	for (i = 0; i < STAGE_HEIGHT; i++)
	{
		Rectangle(hdc, x, y + i * r, x + r, y + i * r + r);
	}
	
	for (i = 0; i < STAGE_WIDTH; i++)
	{
		Rectangle(hdc, x + i * r, y, x + i * r + r, y + r);
	}

	x = x + (i - 1) * r;
	
	for (i = 0; i < STAGE_HEIGHT; i++)
	{
		Rectangle(hdc, x, y + i * r, x + r, y + i * r + r);
	}
}

void drawOneBall(int color, int x, int y, int r)
{	
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 0));
	HGDIOBJ h_old_brush = SelectObject(hdc, hbrush);
	
	Ellipse(hdc, x, y, x + r, y + r);
	
	SelectObject(hdc, h_old_brush);
	DeleteObject(hbrush);
}

void drawMap()
{
	int idx = 0, r = 30;
	float g_fBallSpace = 0.87f;

	// 공이 그려질 위치
	float fDrawX = 1.0f, fDrawY = 1.0f;

	// 반복
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		// 겹치는 수치만큼 이동
		fDrawY = WALL_SIZE + BALL_SIZE * g_fBallSpace * y;

		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			// 공이 없으면 continue
			idx = stage_now[y][x];
			if (idx == 0) continue;

			// 0 : 홀수줄, 1 : 짝수줄
			if (y % 2)
			{
				// 1 : 짝수줄
				// 공의 반만큼 오른쪽으로
				fDrawX = (float)(WALL_SIZE + BALL_SIZE / 2.0f + BALL_SIZE * x);
			}
			else
			{
				// 0 : 짝수줄
				fDrawX = (float)(WALL_SIZE + x * BALL_SIZE);
			}

			drawOneBall(idx, fDrawX, fDrawY, r);
		}
	}
}

void inGame(int map_number)
{
	system("cls");
	PlaySound(TEXT("Puzzle Bobble.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
	int x = 0, y = 0;

	
	int ch = 0;
	
	while(game_active)
	{
		startBuffer();

		if (_kbhit()) if (_getch() == 32) break;
		drawWall();
		drawMap();
		
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
			if (getch() == 13) break;
		}
		startScreen(0);
		Sleep(500);
		system("cls");
		startScreen(1);
		Sleep(500);
		system("cls");
	}

	inGame(1);

	return 0;
}