#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Ground.h"
#include "Candle.h"
#include "Items.h"
#include "Door.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "Textures.h"
#include "VampireBat.h"
#include "FishMan.h"
#include "FireBall.h"
#include "Boss.h"
#include "Stair.h"

Simon::Simon() : GameObject() {

	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSimon = loadResourceFile->GetAnimations("resources\\simon\\simon_ani.xml");
	for each (string animation in animationsSimon)
	{
		AddAnimation(animation);
	}

	weapon = new Weapon();
	nameWeapon = DAGGER_SUB;

	score = 0;
	item = -1;
	energy = 99;
	life = 3;
	HP = 10;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	if (state != STAIR_UP && state != STAIR_DOWN &&
		state != STAIR_UP_ATTACK && state != STAIR_DOWN_ATTACK &&
		isAutoWalk != true)
	{
		if (vy < -0.2f || vy > 0.2f)
			vy += SIMON_GRAVITY * dt;
		else vy += SIMON_GRAVITY_LOWER * dt;

	}

	// Auto walk conditions
	if (isAutoWalk == true)
	{
		if (abs(dx) <= abs(autoWalkDistance))
		{
			x += dx;
			y += dy;
			autoWalkDistance -= dx;
		}
		else
		{
			x += autoWalkDistance;
			state = stateAfterAutoWalk;
			nx = nxAfterAutoWalk;

			SetState(state);
			if (state == STAIR_DOWN) y += 2.0f;

			isAutoWalk = false;
			autoWalkDistance = 0;
			stateAfterAutoWalk = -1;
			nxAfterAutoWalk = 0;
		}

		return; // no need to check collision
	}

	// Reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occurred, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					//vy = 0;
					jumping = false;
					isCollisionWithStair = false;
				}

				if (state == STAIR_UP || state == STAIR_DOWN)
				{
					if (nx != 0) x -= nx * 0.1f;
				}
			}

			// no collision of Simon and Candle
			if (dynamic_cast<Candle*>(e->obj) ||
				dynamic_cast<Stair*>(e->obj) ||
				dynamic_cast<Items*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			else if (dynamic_cast<Door*>(e->obj))
			{
				if (e->obj->GetState() == DOOR_2_IDLE)
				{
					if (e->nx == 1.0f) vx = 0;
					else
					{
						e->obj->SetState(DOOR_2_OPEN);
						e->obj->animations[e->obj->GetState()]->SetAniStartTime(GetTickCount());

						isWalkThroughDoor = true;
					}
				}
				else if (e->obj->GetState() == DOOR_2_OPEN)
				{
					if (nx == -1) vx = 0;
				}
				else
				{
					vx = SIMON_WALKING_SPEED_LOWER;
					vy = 0;
					AutoWalk(80, IDLE, 1);
					return;
				}
			}

			else if (e->obj->GetState() == DESTROYED) x += dx;
			else if (dynamic_cast<Zombie*>(e->obj) ||
				dynamic_cast<BlackLeopard*>(e->obj) ||
				dynamic_cast<VampireBat*>(e->obj) ||
				dynamic_cast<FishMan*>(e->obj) ||
				dynamic_cast<FireBall*>(e->obj) ||
				dynamic_cast<Boss*>(e->obj))
			{
				if (isUntouchable == false)
				{

					// nếu dơi tông trúng simon thì cho huỷ
					if (dynamic_cast<VampireBat*>(e->obj))
					{
						e->obj->vx = 0;
						e->obj->SetState(VAMPIRE_BAT_DESTROYED);
						e->obj->isLastFame = false;
					}

					if (e->nx != 0)
					{
						if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
						else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
					}

					if (!isCollisionWithStair)
					{
						SetState(DEFLECT);
					}

					StartUntouchable();

					//HP = HP - 2;
				}
				else
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}

			else
			{
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (state == STAND_ATTACK || state == SIT_ATTACK || state == STAIR_UP_ATTACK || state == STAIR_DOWN_ATTACK)
	{
		weapon->SetN(nx);
		weapon->SetWeaponPosition(D3DXVECTOR3(x, y, 0), sitting);
	}

	if (!isHitSubWeapons) weapon->Update(dt, coObjects);
}

void Simon::Render()
{
	if (isUntouchable)  // Để render Simon nhấp nháy trong trạng thái isUntouchable
	{
		int r = rand() % 2;

		if (r == 0) animations[state]->Render(nx, x, y);
		else animations[state]->Render(nx, x, y, 100);
	}
	else
	{
		animations[state]->Render(nx, x, y);
	}

	if ((state == STAND_ATTACK || state == SIT_ATTACK || state == STAIR_UP_ATTACK || state == STAIR_DOWN_ATTACK) && (isHitSubWeapons == false))
	{
		weapon->Render();
	}

	standAttacking = !animations[state]->IsCompleted();
	sitAttacking = !animations[state]->IsCompleted();
	throwing = !animations[state]->IsCompleted();
	powering = !animations[state]->IsCompleted();
	stairUpping = !animations[state]->IsCompleted();
	deflecting = !animations[state]->IsCompleted();

	//RenderBoundingBox();
	RenderBBSimon();

	//DebugOut(L"collision with stair %d\n", isCollisionWithStair);
}

void Simon::SetState(string state)
{
	GameObject::SetState(state);

	if (state == IDLE)
	{
		sitting = false;
		isStandOnStair = false;
		isMovingDown = false;
		isMovingUp = false;
		vx = 0;
	}

	else if (state == WALK)
	{
		sitting = false;
		isStandOnStair = false;
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
	}

	else if (state == JUMP)
	{
		jumping = true;
		sitting = false;
		isStandOnStair = false;
		vy = -SIMON_JUMP_SPEED_Y;
	}

	else if (state == STAND_ATTACK)
	{
		sitting = false;
		isStandOnStair = false;
		animations[state]->Reset();
		weapon->animations[weapon->GetState()]->Reset();
	}

	else if (state == SIT_ATTACK)
	{
		sitting = true;
		isStandOnStair = false;
		animations[state]->Reset();
		weapon->animations[weapon->GetState()]->Reset();
	}

	else if (state == SIT)
	{
		sitting = true;
		isStandOnStair = false;
		vx = 0;
		vy = 0;
	}

	else if (state == POWER)
	{
		isStandOnStair = false;
		vx = 0;
	}

	else if (state == STAIR_UP)
	{
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = -SIMON_STAIR_SPEED_Y;
		animations[state]->Reset();
		animations[state]->SetAniStartTime(GetTickCount());
	}

	else if (state == STAIR_DOWN)
	{
		if (nx > 0) vx = SIMON_STAIR_SPEED_X;
		else vx = -SIMON_STAIR_SPEED_X;
		vy = SIMON_STAIR_SPEED_Y;
		animations[state]->Reset();
		animations[state]->SetAniStartTime(GetTickCount());
	}

	else if (state == DEFLECT)
	{
		vy = -SIMON_DEFLECT_SPEED_Y;
		if (nx > 0) vx = -SIMON_DEFLECT_SPEED_X;
		else vx = SIMON_DEFLECT_SPEED_X;
	}

	else if (state == STAIR_UP_ATTACK || state == STAIR_DOWN_ATTACK)
	{
		weapon->animations[weapon->GetState()]->Reset();
		sitting = false;
		vx = 0;
		vy = 0;
		animations[state]->Reset();
		animations[state]->SetAniStartTime(GetTickCount());
	}

}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 15; //30,60
	top = y + 2;  //62,66
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;

}

#pragma region CheckState

bool Simon::IsJumping()
{
	return (state == JUMP && jumping);
}

bool Simon::IsStandAttacking()
{
	return (state == STAND_ATTACK && standAttacking);
}

bool Simon::IsSitAttacking()
{
	return (state == SIT_ATTACK && sitAttacking);
}

bool Simon::IsThrowing()
{
	return (state == THROW && throwing);
}

bool Simon::IsPowering()
{
	return (state == POWER && powering);
}

bool Simon::IsDeflecting()
{
	return (state == DEFLECT && deflecting);
}

bool Simon::IsStairUpping()
{
	return (state == STAIR_UP && stairUpping);
}

void Simon::RenderBBSimon()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	t += 55;
	b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống
	//l -= 5;
	//r += 5;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	Game::GetInstance()->Draw(1, l, t, bbox, 0, 0, rect.right, rect.bottom, 100);
}

#pragma endregion CheckState

void Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	float simonLeft, simonTop, simonRight, simonBottom;
	GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

	//thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simonTop += 55;
	simonBottom += 10;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống
	simonLeft += 5;
	simonRight -= 5;
	for (UINT i = 0; i < listStair->size(); i++)
	{
		if (listStair->at(i)->GetType() == "BOTTOM")
		{
			float stair_l, stair_t, stair_r, stair_b;
			listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);
			if (Game::AABB(simonLeft, simonTop, simonRight, simonBottom, stair_l, stair_t, stair_r, stair_b))
			{
				if (listStair->at(i)->GetState() == STAIR_LEFT_UP) stairDirection = 1;
				else stairDirection = -1;

				stairCollided = listStair->at(i);
				isCollisionWithStair = true;
				isMovingUp = true;
				if (simonBottom > stair_b)
				{
					isMovingDown = false;
				}
				else if (simonTop - 10 < stair_t) // -10 have it to moving down first stair
				{
					isMovingDown = true;
				}
				break;
			}
			/*	else if (simonBottom < stair_t)
				{
					isCollisionWithStair = true;
				}*/
				/*	else if (simonBottom > stair_b && simonRight < stair_l) {
						isCollisionWithStair = false;
					}*/
		}
		else if (listStair->at(i)->GetType() == "TOP")
		{
			float stair_l, stair_t, stair_r, stair_b;
			listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);

			if (Game::AABB(simonLeft, simonTop, simonRight, simonBottom, stair_l, stair_t, stair_r, stair_b))
			{
				if (listStair->at(i)->GetState() == STAIR_LEFT_UP) stairDirection = 1;
				else stairDirection = -1;

				stairCollided = listStair->at(i);
				isCollisionWithStair = true;
				isMovingDown = true;
				isMovingUp = false;
				if (simonBottom > stair_b)
				{
					isMovingUp = true;
				}
				break;
			}
			/*	else if (simonBottom > stair_t_ && simonLeft > stair_r_)
				{
					isCollisionWithStair = false;
				}*/
		}
	}
}

// Kiểm tra va chạm với danh sách item
bool Simon::CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem)
{
	float simonLeft, simonTop, simonRight, simonBottom;
	float itemLeft, itemTop, itemRight, itemBottom;

	GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

	for (UINT i = 0; i < listItem->size(); i++)
	{
		if (listItem->at(i)->IsEnable() == false)
			continue;

		listItem->at(i)->GetBoundingBox(itemLeft, itemTop, itemRight, itemBottom);

		if (this->AABBx(listItem->at(i)))
		{
			listItem->at(i)->isEnable = false;

			string nameItem = listItem->at(i)->GetState();

			if (nameItem == CROSS || nameItem == DAGGER || nameItem == AXE || nameItem == HOLY_WATER || nameItem == BOOMERANG)
			{
				nameWeapon = ItemToSubWeapon(nameItem);
			}

			else if (nameItem == SMALL_HEART)
			{
				energy += 1;
			}

			else if (nameItem == LARGE_HEART)
			{
				energy += 5;

			}

			else if (nameItem == CHAIN)
			{
				SetState(POWER); // đổi trạng thái power - biến hình nhấp nháy các kiểu đà điểu
				vx = 0;
				// lên đời whip
				if (weapon->GetState() == MAGIC_WHIP) weapon->SetState(SHORT_CHAIN);
				else if (weapon->GetState() == SHORT_CHAIN) weapon->SetState(LONG_CHAIN);
			}

			else if (nameItem == MONEY_BAG_RED)
			{
				score += 100;
			}

			return true;
		}
	}
}

// Kiểm tra va chạm với vùng hoạt động của enemy
void Simon::CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listEnemy)
{
	float simonLeft, simonTop, simonRight, simonBottom;

	GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

	for (UINT i = 0; i < listEnemy->size(); i++)
	{
		LPGAMEOBJECT enemy = listEnemy->at(i);

		// Không cần xét vùng active nữa khi nó đang active / destroyed
		if (enemy->GetState() == ZOMBIE_ACTIVE ||
			enemy->GetState() == BLACK_LEOPARD_ACTIVE ||
			enemy->GetState() == VAMPIRE_BAT_ACTIVE ||
			enemy->GetState() == ZOMBIE_ACTIVE ||
			enemy->GetState() == DESTROYED)
			continue;

		float enemyLeft, enemyTop, enemyRight, enemyBottom;
		enemy->GetActiveBoundingBox(enemyLeft, enemyTop, enemyRight, enemyBottom);

		if (Game::AABB(simonLeft, simonTop, simonRight, simonBottom, enemyLeft, enemyTop, enemyRight, enemyBottom) == true)
		{
			D3DXVECTOR2 enemyEntryPostion = enemy->GetEntryPosition();

			if (dynamic_cast<Zombie*>(enemy))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(enemy);

				if (zombie->IsAbleToActivate() == true)
				{
					if (enemyEntryPostion.x < x) zombie->SetN(1);
					else zombie->SetN(-1);

					zombie->SetState(ZOMBIE_ACTIVE);
				}
			}
			else if (dynamic_cast<BlackLeopard*>(enemy))
			{
				BlackLeopard* leopard = dynamic_cast<BlackLeopard*>(enemy);
				if (leopard->GetState() == BLACK_LEOPARD_IDLE)
					leopard->SetState(BLACK_LEOPARD_ACTIVE);
			}
			else if (dynamic_cast<VampireBat*>(enemy))
			{
				VampireBat* bat = dynamic_cast<VampireBat*>(enemy);

				if (bat->GetState() == VAMPIRE_BAT_INACTIVE && bat->IsAbleToActivate() == true)
				{
					bat->SetState(VAMPIRE_BAT_ACTIVE);
				}
			}
			else if (dynamic_cast<FishMan*>(enemy))
			{
				FishMan* fishman = dynamic_cast<FishMan*>(enemy);

				if (fishman->GetState() == FISHMAN_INACTIVE && fishman->IsAbleToActivate() == true)
				{
					fishman->SetState(FISHMAN_JUMP);
				}
			}
			else if (dynamic_cast<Boss*>(enemy))
			{
				Boss* boss = dynamic_cast<Boss*>(enemy);
				boss->SetState(BOSS_ACTIVE);
			}
		}
	}
}

// Kiểm tra va chạm với trigger change scene
bool Simon::CheckChangeScene(vector<LPCHANGESCENEOBJ>* listChangeScene)
{
	float simonLeft, simonTop, simonRight, simonBottom;
	float objLeft, objTop, objRight, objBottom;

	GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

	for (UINT i = 0; i < listChangeScene->size(); i++)
	{
		listChangeScene->at(i)->GetBoundingBox(objLeft, objTop, objRight, objBottom);

		float t, nx, ny;
		Game::SweptAABB(simonLeft, simonTop, simonRight, simonBottom, dx, dy, objLeft, objTop, objRight, objBottom, t, nx, ny);
		bool collision = Game::AABB(simonLeft, simonTop, simonRight, simonBottom, objLeft, objTop, objRight, objBottom);

		if (nx != 0 || ny != 0 || collision == true)
		{
			changeScene = listChangeScene->at(i)->GetIDNextScene();
			return true;
		}
	}

	return false;
}

void Simon::StandOnStair()
{
	vx = 0;
	vy = 0;
}

void Simon::AutoWalk(float distance, string new_state, int new_nx)
{
	isAutoWalk = true;

	autoWalkDistance = distance;
	stateAfterAutoWalk = new_state;
	nxAfterAutoWalk = new_nx;
}
