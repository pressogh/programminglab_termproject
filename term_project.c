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

typedef struct _Launcher
{
	int x;
	int y;
	float angle;
	int size;
}_Launcher;

//
// 맵 관련
//

// 저장된 맵
int stage_saved[STAGE_COUNT][STAGE_HEIGHT][STAGE_WIDTH] = 
{
	{
		1, 1, 2, 2, 3, 3, 4, 4,
		1, 1, 2, 2, 3, 3, 4, 0,
		3, 7, 4, 4, 1, 1, 2, 2,
		3, 0, 4, 4, 1, 1, 2, 0,
		1, 0, 6, 6, 5, 5, 4, 4,
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

int stage = 0;
bool game_active = true;
int stageNow[STAGE_HEIGHT][STAGE_WIDTH] = {
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

_Launcher L;
_LaunchBall LB = { 0, 0, 0, 0, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f };


void init()
{
	system("mode con cols=150 lines=50");
	srand(time(NULL));
	removeCursor();

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			stageNow[i][j] = stage_saved[0][i][j];
		}
	}
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
	int x = 0, y = 0;
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

void drawOneBall(int color,	float x, float y)
{
	int brushCol, penCol;
	if (color == 1)
	{
		brushCol = RGB(255, 99, 99);
		penCol = RGB(200, 51, 51);
	}
	else if (color == 2)
	{
		brushCol = RGB(99, 255, 99);
		penCol = RGB(52, 193, 49);
	}
	else if (color == 3)
	{
		brushCol = RGB(99, 99, 255);
		penCol = RGB(50, 50, 201);
	}
	else if (color == 4)
	{
		brushCol = RGB(255, 255, 51);
		penCol = RGB(209, 164, 51);
	}
	else if (color == 5)
	{
		brushCol = RGB(255, 99, 255);
		penCol = RGB(196, 49, 196);
	}
	else if (color == 6)
	{
		brushCol = RGB(99, 255, 255);
		penCol = RGB(49, 201, 200);
	}
	else if (color == 7)
	{
		brushCol = RGB(145, 145, 145);
		penCol = RGB(46, 46, 46);
	}
	
	HBRUSH hbrush = CreateSolidBrush(brushCol);
	HGDIOBJ h_old_brush = SelectObject(hdc, hbrush);

	HPEN hpen = CreatePen(PS_SOLID, 3, penCol);
	HPEN h_old_pen = (HPEN)SelectObject(hdc, hpen);
	
	Ellipse(hdc, x, y, x + BALL_SIZE, y + BALL_SIZE);
	
	SelectObject(hdc, h_old_brush);
	DeleteObject(hbrush);
	SelectObject(hdc, h_old_pen);
	DeleteObject(hpen);
}

void drawMap()
{
	int idx = 0;
	float ballSpace = 0.87f;

	// 공이 그려질 위치
	float fDrawX = 0.0f, fDrawY = 0.0f;

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		fDrawY = WALL_SIZE + BALL_SIZE * ballSpace * i;
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			idx = stageNow[i][j];
			if (idx == 0) continue;
			
			if (i % 2) fDrawX = (float)(WALL_SIZE + BALL_SIZE / 2.0f + BALL_SIZE * j);
			else fDrawX = (float)(WALL_SIZE + j * BALL_SIZE);
			drawOneBall(idx, fDrawX, fDrawY);
		}
	}
}

void drawBallInLauncher()
{
	float x = 225, y = 50 * STAGE_HEIGHT;

	switch (LB.ballState)
	{
	case BALL_STATE_READY:
	{
		drawOneBall(LB.ballNow, x, y);
	}
	break;

	case BALL_STATE_ING:
	{

		TextOut(hdc, 0, 0, "fjeoiwafjoewjfoa", strlen("fjeoiwafjoewjfoa"));
		drawOneBall(LB.ballNow, LB.flyingBallx, LB.flyingBally);
	}
	break;
	default: break;
	}

	drawOneBall(LB.ballNext, x - 100, y);
}

void drawLauncher()
{	
	float r = L.angle * PI / 180.0f;
	float c = cos(r), s = sin(r);

	MoveToEx(hdc, L.x, L.y, NULL);
	LineTo(hdc, L.x + L.size * c, L.y + L.size * s);
	
	drawBallInLauncher();
}

void selectNewBall()
{
	if (LB.ballNow == 0)
	{
		int arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < STAGE_HEIGHT; i++)
		{
			for (int j = 0; j < STAGE_WIDTH; j++)
			{
				if (stageNow[i][j] != 0) arr[stageNow[i][j]]++;
			}
		}

		while (1)
		{
			int tmp = 1 + rand() % 7;
			LB.ballNext = tmp;
			if (arr[tmp] != 0) break;
		}
	}

	LB.ballNow = LB.ballNext;
	
	int arr[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (stageNow[i][j] != 0) arr[stageNow[i][j]]++;
		}
	}
	
	while(1)
	{
		int tmp = 1 + rand() % 7;
		LB.ballNext = tmp;
		if (arr[tmp] != 0) break;
	}

	LB.ballState = BALL_STATE_READY;
	LB.ballCanFire = true;
}

bool stageCheck[STAGE_HEIGHT][STAGE_WIDTH] = { false };
void checkAirBall(int x, int y)
{
	int tmp = 0;

	if (x < 0 || y < 0) return;
	if (x >= STAGE_WIDTH || y >= STAGE_HEIGHT) return;
	if (y % 2)
	{
		if (x >= STAGE_WIDTH - 1) return;
	}

	if (stageNow[y][x] == 0) return;
	if (stageCheck[y][x]) return;
	stageCheck[y][x] = true;

	if (y != 0)
	{
		checkAirBall(x - 1, y);
		checkAirBall(x + 1, y);
	}

	if (y % 2)
	{
		checkAirBall(x, y + 1);
		checkAirBall(x + 1, y + 1);
	}
	else
	{
		checkAirBall(x - 1, y + 1);
		checkAirBall(x, y + 1);
	}
}

void airBallDrop()
{
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			stageCheck[i][j] = false;
		}
	}

	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		checkAirBall(i, 0);
	}

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (stageCheck[i][j]) continue;
			stageNow[i][j] = 0;
		}
	}
}

bool ballFlyingCheck()
{
	float ballSpace = 0.86f;

	int x = 0, y = 0;
	float ballSize = BALL_SIZE, wallSize = WALL_SIZE;

	float enc = ballSize * ballSpace;	// 공끼리 접하는 수치
	float sx = 0.0f, sy = 0.0f;			// 보드에 있는 공의 중심 좌표
	float dx = 0.0f, dy = 0.0f;			// 날라가는 공의 중심 좌표
	float xx = 0.0f, yy = 0.0f, zz = 0.0f;

	if (LB.flyingBally <= wallSize) return true;

	for (y = 0; y < STAGE_HEIGHT; y++)
	{
		for (x = 0; x < STAGE_WIDTH; x++)
		{
			if (stageNow[y][x] == 0) continue;

			if (y % 2) // 짝수 줄
			{
				sx = wallSize + (x * ballSize) + ballSize;
			}
			else // 홀수 줄
			{
				sx = wallSize + (x * ballSize) + ballSize * 0.5f;
			}

			sy = wallSize + (y * enc) + (ballSize * 0.5f);

			dx = LB.flyingBallx + ballSize * 0.5f;
			dy = LB.flyingBally + ballSize * 0.5f;

			xx = dx - sx;
			yy = dy - sy;
			zz = enc;

			if ((xx * xx + yy * yy) <= zz * zz)
			{
				return true;
			}
		}
	}

	return false;
}

void ballFlying()
{
	float wallLeft = WALL_SIZE;
	float wallRight = WALL_SIZE + BALL_SIZE * STAGE_WIDTH;

	LB.flyingBallx += LB.flyingBallMoveX;
	LB.flyingBally -= LB.flyingBallMoveY;

	if (LB.flyingBallx < wallLeft)
	{
		LB.flyingBallx = wallLeft + (wallLeft - LB.flyingBallx);
		
		LB.flyingBallMoveX *= -1.0f;
	}

	if (LB.flyingBallx + BALL_SIZE > wallRight)
	{
		LB.flyingBallx = wallRight - (LB.flyingBallx + BALL_SIZE - wallRight);
		LB.flyingBallx -= BALL_SIZE;

		LB.flyingBallMoveX *= -1.0f;
	}

	if (ballFlyingCheck())
	{
		LB.ballState = BALL_STATE_END;
	}
}

void insertBallInStage()
{
	float ballSize = BALL_SIZE, wallSize = WALL_SIZE;
	float fx = 0.0f, fy = 0.0f;
	int nx = 0, ny = 0;
	
}

void workBall()
{
	switch(LB.ballState)
	{
		case BALL_STATE_ING:
		{
			ballFlying();
		}
		break;

		case BALL_STATE_END:
		{
			//insertBallInStage();
			//deleteBallNotInCeiling();
			//findEndGame();
			selectNewBall();
		}
		break;
		
		case BALL_STATE_EMPTY: break;
		case BALL_STATE_READY: break;
	}
}

void launchBall()
{
	if (LB.ballState != BALL_STATE_READY || !LB.ballCanFire) return;
	LB.flyingBallx = 225;
	LB.flyingBally = 50 * STAGE_HEIGHT;

	DOUBLE r = L.angle * PI / 180.0f;
	LB.flyingBallMoveX = LB.flyingBallSpeed * (float)cos(r);
	LB.flyingBallMoveY = -1.0 * LB.flyingBallSpeed * (float)sin(r);

	LB.ballState = BALL_STATE_ING;
	LB.ballCanFire = false;
}

void workLauncher(int ch)
{
	if (ch == 75) L.angle -= 2;
	else if (ch == 77) L.angle += 2;
	else if (ch == 72)
	{
		launchBall();
	}

	// 발사대 각도 처리
	if (L.angle <= -180.0f) {
		L.angle = -178.0f;
	}
	else if (L.angle >= 0.0f)
	{
		L.angle = -2.0f;
	}
}

void ballInEnd()
{
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		if (stageNow[10][i] != 0) game_active = false;
	}

	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		for (int j = 0; j < STAGE_WIDTH; j++)
		{
			if (stageNow[i][j] != 0) return;
		}
	}

	stage++;
	// initGameData();
}



void inGame()
{
	system("cls");
	PlaySound(TEXT("Puzzle Bobble.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
	
	L.x = 250;
	L.y = 50 * STAGE_HEIGHT + 25;
	L.angle = -90.0f;
	L.size = 50;
	
	selectNewBall();
	
	while(game_active)
	{
		startBuffer();

		drawWall();
		drawMap();
		drawLauncher();

		workBall();
		if (_kbhit())
		{
			int tmp;
			tmp = _getch();
			
			if (tmp == 32) game_active = false;
			else if (tmp == 224)
			{
				int ch = _getch();
				workLauncher(ch);
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