#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "LoadResourceFile.h"

#define SIMON_WALKING_SPEED		0.15f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	100
#define SIMON_STATE_WALKING_LEFT	200
#define SIMON_STATE_JUMP			300
#define SIMON_STATE_DIE				400
#define SIMON_STATE_SIT				500
#define SIMON_STATE_ATTACK		600

#define SIMON_ANI_BIG_IDLE_RIGHT		0
#define SIMON_ANI_BIG_IDLE_LEFT			0
#define SIMON_ANI_SMALL_IDLE_RIGHT		2
#define SIMON_ANI_SMALL_IDLE_LEFT			3

#define SIMON_ANI_BIG_WALKING_RIGHT			4
#define SIMON_ANI_BIG_WALKING_LEFT			1
#define SIMON_ANI_SMALL_WALKING_RIGHT		6
#define SIMON_ANI_SMALL_WALKING_LEFT		7

#define SIMON_ANI_DIE				8

#define	SIMON_LEVEL_SMALL	1
#define	SIMON_LEVEL_BIG		2

#define SIMON_BIG_BBOX_WIDTH 60 
#define SIMON_BIG_BBOX_HEIGHT 63 

#define SIMON_UNTOUCHABLE_TIME 5000


class Simon : public GameObject
{
	int level;
	int untouchable;

	bool jumping = false;
	bool sitting = false;
	bool attacking = false;
	Whip* whip;

	DWORD untouchable_start;

public:

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	int GetY() { return y; }
	int GetVy() { return vy; }

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsAttacking();
	
};