#pragma once
#include "GameObject.h"
#include "Weapon.h"
#include "LoadResourceFile.h"
#include "define.h"
#include "SubWeapon.h"
#include "ChangeSceneObject.h"
#include "Stair.h"
#include "Timer.h"

class Simon : public GameObject
{
	Weapon *weapon;

	string nameWeapon;
	int score;
	string item;
	int energy;
	int life;
	int HP;

	// auto-walk
	float autoWalkDistance = 0;
	string stateAfterAutoWalk = "";
	int nxAfterAutoWalk = 0;

public:

	Timer* untouchableTimer = new Timer(SIMON_UNTOUCHABLE_TIME);
	Timer* invisibilityTimer = new Timer(SIMON_INVISIBILITY_TIME);

	bool jumping = false;
	bool sitting = false;
	bool standAttacking = false;
	bool sitAttacking = false;
	bool throwing = false;
	bool powering = false;
	bool stairUpping = false;
	bool deflecting = false;

	bool isDead = false;
	bool isTouchGround = false;
	bool isFalling = false;
	bool isFallingWater = false;
	bool isStandOnStair = false;	// trạng thái đang đứng trên cầu thang 
	bool canMoveUpStair = false;	// có thể di chuyển lên cầu thang
	bool canMoveDownStair = false;	// có thể di chuyển xuống cầu thang
	bool isAutoWalk = false;		// tự động đi
	bool isWalkThroughDoor = false;	// đi qua cửa
	bool isHitSubWeapons = false;	// xác định xem là hit bằng roi hay subweapon
	bool isGotChainItem = false;	// xác định xem có nhặt được Chain item hay không, dùng để update whip
	bool isGotDoubleShotItem = false; // Double shot item
	bool isGotTripleShotItem = false; // Triple shot item
	bool isGotCrossItem = false;
	bool isCollisionWithStair = false;

	int changeScene = -1;			// lưu id Scene kế tiếp khi Simon va chạm với ChangeSceneObject

	int stairDirection = 0;			// 1: trái dưới - phải trên, -1: trái trên - phải dưới

	LPGAMEOBJECT stairCollided = nullptr; // lưu bậc thang va chạm với simon -> để xét vị trí cho chuẩn trong hàm PositionCorrection

	///
	Simon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(string state);

	virtual void GetBoundingBoxFoot(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsJumping();
	bool IsSitting() { return sitting; }
	bool IsStandAttacking();
	bool IsSitAttacking();
	bool IsThrowing();
	bool IsPowering();
	bool IsDeflecting();
	bool IsStairUpping();

	// Get function
	int GetEnergy() { return this->energy; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetHP() { return this->HP; }
	string GetItem() { return this->item; }
	Weapon* GetWeapon() { return this->weapon; }
	string GetSubWeapon() { return this->nameWeapon; }
	string ItemToSubWeapon(string itemName) { return itemName + "_SUB"; }

	// Properties change
	void AddScore(int x) { score += x; }
	void LoseEnergy(int x) { energy -= x; }
	void LoseHP(int x);
	void SetHP(int x) { HP = x; }
	void SetEnergy(int x) { energy = x; }
	void SetSubWeapon(string x) { nameWeapon = x; }

	// Kiểm tra va chạm với danh sách bậc thang
	void CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);

	// Kiểm tra va chạm với danh sách item
	bool CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem);

	// Kiểm tra va chạm với vùng hoạt động của enemy
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects);

	// Giữ cho Simon đứng yên trên bậc thang
	void StandOnStair() { vx = vy = 0; }

	// Auto-walk
	void AutoWalk(float distance, string new_state, int new_nx);
	void DoAutoWalk();

	// Xác định trạng thái đang đánh
	bool IsHit();
};