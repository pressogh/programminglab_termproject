#pragma once

#define STAGE_COUNT 3
#define STAGE_WIDTH 8
#define STAGE_HEIGHT 11
#define WALL_SIZE 50
#define BALL_COUNT 7
#define BALL_SIZE 50

#define LAUNCHER_WIDTH 32
#define LAUNCHER_HEIGHT 44

#define PI 3.141592f

enum
{
	BALL_STATE_EMPTY = 0,
	BALL_STATE_READY,
	BALL_STATE_ING,
	BALL_STATE_END
};

typedef struct _LaunchBall
{
	int ballCanFire;
	int ballState;
	int ballNow;
	int ballNext;
	float flyingBallx;
	float flyingBally;
	float flyingBallSpeed;
	float flyingBallMoveX;
	float flyingBallMoveY;
}_LaunchBall;