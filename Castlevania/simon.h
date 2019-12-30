#pragma once
#include "GameObject.h"
#include "Weapon.h"
#include "LoadResourceFile.h"
#include "define.h"
#include "SubWeapon.h"
#include "ChangeSceneObject.h"
#include "Stair.h"

class Simon : public GameObject
{
	Weapon *weapon;
	string nameWeapon;

	int score;
	string item;
	int energy;
	int life;
	int HP;

	float autoWalkDistance = 0;
	string stateAfterAutoWalk = "";
	int nxAfterAutoWalk = 0;

	bool isUntouchable = false;
	DWORD untouchable_start = 0;

	bool isHitSubWeapons = false; // xác định xem là hit bằng roi hay subweapon

	bool isGotDoubleShotItem = false;
	bool isGotTripleShotItem = false;

	bool isDead = false;

	bool isCrossCollected = false;

public:

	bool jumping = false;
	bool sitting = false;
	bool standAttacking = false;
	bool sitAttacking = false;
	bool throwing = false;
	bool powering = false;
	bool stairUpping = false;
	bool deflecting = false;

	bool isPowered = false;
	bool isAutoWalk = false;
	bool isWalkThroughDoor = false;
	bool isTouchGround = false;
	bool isStandOnStair = false; // trạng thái đang đứng trên cầu thang 
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isCollisionWithStair = false;
	int stairDirection = 0; // 1: trái dưới - phải trên, -1: trái trên - phải dưới


	int changeScene = -1;

	LPGAMEOBJECT stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí cho chuẩn trong hàm PositionCorrection

	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBoxFoot(float& left, float& top, float& right, float& bottom);

	void SetState(string state);

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsStandAttacking();
	bool IsSitAttacking();
	bool IsThrowing();
	bool IsPowering();
	bool IsDeflecting();
	bool IsStairUpping();

	int GetEnergy() { return this->energy; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	string GetItem() { return this->item; }
	int GetHP() { return this->HP; }
	Weapon* GetWeapon() { return this->weapon; }
	string GetSubWeapon() { return this->nameWeapon; }
	string ItemToSubWeapon(string itemName) { return itemName + "_SUB"; }
	int GetStairDirection() { return this->stairDirection; }
	void RenderBBSimon();

	int GetChangeScene() { return this->changeScene; }
	void SetChangeScene(int x) { this->changeScene = x; }

	bool IsTouchGround() { return isTouchGround; }

	bool IsStandOnStair() { return this->isStandOnStair; }
	void SetStandOnStair(bool x) { this->isStandOnStair = x; }

	bool IsMovingUp() { return this->isMovingUp; }
	bool IsMovingDown() { return this->isMovingDown; }

	void LoseEnergy(int amount) { energy -= amount; }
	void LoseHP(int x);
	void SetHP(int x) { HP = x; }
	void SetEnergy(int x) { energy = x; }

	void CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }

	bool CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem);
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listEnemy);
	bool CheckChangeScene(vector<LPCHANGESCENEOBJ>* listChangeScene);

	// Giữ cho Simon đứng yên trên bậc thang
	void StandOnStair();

	void AutoWalk(float distance, string new_state, int new_nx);
	bool IsAutoWalk() { return this->isAutoWalk; }

	void StartUntouchable() { isUntouchable = true; untouchable_start = GetTickCount(); }

	bool IsHitSubWeapons() { return isHitSubWeapons; }
	void SetHitSubWeapons(bool x) { isHitSubWeapons = x; }

	void SetSubWeapon(string x) { nameWeapon = x; }

	bool IsGotDoubleShotItem() { return isGotDoubleShotItem; }
	void SetGotDoubleShotItem(bool x) { isGotDoubleShotItem = x; }

	bool IsGotTripleShotItem() { return isGotTripleShotItem; }
	void SetGotTripleShotItem(bool x) { isGotTripleShotItem = x; }

	bool IsDead() { return isDead; }

	bool IsCrossCollected() { return isCrossCollected; }
	void SetCrossCollected(bool x) { isCrossCollected = x; }

};