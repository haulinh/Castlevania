#pragma once
#include "GameObject.h"
#include "Weapon.h"
#include "LoadResourceFile.h"
#include "define.h"

class Simon : public GameObject
{
	Weapon *weapon;

	DWORD untouchable_start;

public:


	int level;
	int untouchable;

	bool jumping = false;
	bool sitting = false;
	bool attacking = false;
	bool throwing = false;

	bool isPowered = false;

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	//D3DXVECTOR3 GetPosition() { return D3DXVECTOR3(x, y, 0); }

	void SetState(string state);
	void SetLevel(int l) { level = l; }

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsAttacking();

	bool IsThrowing();
	
};