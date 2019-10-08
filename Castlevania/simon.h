#pragma once
#include "GameObject.h"
#include "Whip.h"
#include "LoadResourceFile.h"
#include "define.h"

class Simon : public GameObject
{
	int level;
	int untouchable;

	bool jumping = false;
	bool sitting = false;
	bool attacking = false;

	DWORD untouchable_start;

public:

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	//D3DXVECTOR3 GetPosition() { return D3DXVECTOR3(x, y, 0); }

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsAttacking();
	
};