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

#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

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
		0, 0, 0, 0, 0, 0, 5, 0,
		2, 1, 1, 7, 7, 6, 6, 5,
		2, 0, 0, 0, 0, 0, 0, 0,
		3, 3, 4, 4, 5, 5, 6, 6,
		0, 0, 0, 0, 0, 0, 7, 0,
		0, 3, 3, 2, 2, 1, 1, 7,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	}
};

// 현재 맵
int stage_now[STAGE_WIDTH * STAGE_HEIGHT] = { 0 };
// 현재 맵 번호
int stage_now_num = 0;
// 공 줄간격
float ballspace = 0.87f;
// 맵 체크
bool stage_check[STAGE_HEIGHT * STAGE_WIDTH] = { false };
int stage_ycheck[STAGE_HEIGHT * STAGE_WIDTH] = { 0 };

//
// 공 관련
//

// 발사 가능 여부
bool can_launch = false;
// 발사될 공 상태
int ball_state = 0;
// 발사되는 공 색, 다음 공 색
int ball_now = 0, ball_next = 0;
// 날아가는 공 좌표
float flying_ball_x = 0.0f, flying_ball_y = 0.0f;
// 날아가는 공 속력
float flying_ball_speed = 0.0f;
// 날아가는 공의 x, y 이동 수치
float flying_ball_move_x = 0.0f, flying_ball_move_y = 0.0f;
int ball_radius = 30;

//
// 발사대 관련
//

// 발사대 각도
float launcher_angle = 90.0f;
// 발사대 각도 최소, 최대
float launcher_angle_min = 15.0f, launcher_angle_max = 175.0f;
// 발사대 위치
float launcher_x = 0.0f, launcher_y = 0.0f;
// 발사대 오프셋 좌표
float launcher_offset_x = 16.0f, launcher_offset_y = 16.0f;

//
// 게임 상황
//

bool game_active = false;

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
static HWND hWnd;

void startBuffer()
{
	hWnd = GetForegroundWindow();
	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &bufferRT);
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
	GetClientRect(hWnd, &bufferRT);
	BitBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, MemDC, 0, 0, SRCCOPY);
	SelectObject(MemDC, oldBackBit);
	DeleteObject(BackBit);
	DeleteDC(MemDC);
	ReleaseDC(hWnd, hdc);
	EndPaint(hWnd, &ps);
}

void inGame(int map_number)
{
	system("cls");
	PlaySound(TEXT("Puzzle Bobble.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
	int x = 0, y = 0;

	int ch = 0;
	while(!game_active)
	{
		startBuffer();
		
		if (_kbhit())
		{
			int temp = _getch();
			if (temp == 32) break;
			if (temp == 224) ch = _getch();
		}
		
		if (ch == 72) y--;
		else if (ch == 80) y++;
		else if (ch == 75) x--;
		else if (ch == 77) x++;

		Ellipse(hdc, x, y, x+50, y+50);
		
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