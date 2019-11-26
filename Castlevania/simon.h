#pragma once
#include "GameObject.h"
#include "Weapon.h"
#include "LoadResourceFile.h"
#include "define.h"
#include "SubWeapon.h"

class Simon : public GameObject
{
	Weapon *weapon;
	string nameWeapon;

	int score;
	int item;
	int energy;
	int life;
	int HP;

public:

	bool jumping = false;
	bool sitting = false;
	bool standAttacking = false;
	bool sitAttacking = false;
	bool throwing = false;
	bool powering = false;

	bool isPowered = false;

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(string state);

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsStandAttacking();
	bool IsSitAttacking();
	bool IsThrowing();
	bool IsPowering();

	int GetEnergy() { return this->energy; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetItem() { return this->item; }
	string GetSubWeapon() { return this->nameWeapon; }
	int GetHP() { return this->HP; }

	void LoseEnergy(int amount) { energy -= amount; }
};