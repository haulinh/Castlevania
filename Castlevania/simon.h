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
	bool stairUpping= false;

	bool isPowered = false;

	bool isStandOnStair = false; // trạng thái đang đứng trên cầu thang 
	bool isMovingUp = false;
	bool isMovingDown = false;
	int stairDirection = 0; // 1: trái dưới - phải trên, -1: trái trên - phải dưới

	LPGAMEOBJECT stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí cho chuẩn trong hàm PositionCorrection

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
	bool IsStairUpping();

	int GetEnergy() { return this->energy; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetItem() { return this->item; }
	int GetHP() { return this->HP; }
	string GetSubWeapon() { return this->nameWeapon; }
	string ItemToSubWeapon(string itemName) { return itemName + "_SUB"; }
	int GetStairDirection() { return this->stairDirection; }

	bool IsStandOnStair() { return this->isStandOnStair; }
	void SetStandOnStair(bool x) { this->isStandOnStair = x; }

	bool IsMovingUp() { return this->isMovingUp; }
	bool IsMovingDown() { return this->isMovingDown; }

	void LoseEnergy(int amount) { energy -= amount; }

	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }

	// Căn chỉnh lại vị trí của Simon với bậc thang
	void PositionCorrection(string prevState = "");  // -1 is not changed  
	// Giữ cho Simon đứng yên trên bậc thang
	void StandOnStair();
};