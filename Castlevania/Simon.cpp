#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Ground.h"
#include "Candle.h"
#include "Items.h"
#include "Door.h"
#include "Zombie.h"

Simon::Simon() : GameObject() {

	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSimon = loadResourceFile->GetAnimations("resources\\simon\\simon_ani.xml");
	for each (string animation in animationsSimon)
	{
		AddAnimation(animation);
	}

	weapon = new Weapon();
	nameWeapon = BOOMERANG_SUB;

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

	if (state != STAIR_UP && state != STAIR_DOWN && isAutoWalk != true)
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

			// no collision of Simon and Candle
			if (dynamic_cast<Candle*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					jumping = false;
				}

				if (state == STAIR_UP || state == STAIR_DOWN)
				{
					if (nx != 0) x -= nx * 0.1f;
				}
			}

			else if (dynamic_cast<Door*>(e->obj))
			{
				if (e->obj->GetState() == DOOR_2_IDLE)
				{
					e->obj->SetState(DOOR_2_OPEN);
					e->obj->animations[e->obj->GetState()]->SetAniStartTime(GetTickCount());

					isWalkThroughDoor = true;
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
				}
			}

			else if (dynamic_cast<Zombie*>(e->obj))
			{
				if (isUntouchable == false)
				{
					if (e->nx != 0)
					{
						if (e->nx == 1.0f && this->nx == 1) this->nx = -1;
						else if (e->nx == -1.0f && this->nx == -1) this->nx = 1;
					}

					SetState(DEFLECT);
					StartUntouchable();

					//HP = HP - 2;
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

	if (state == STAND_ATTACK || state == SIT_ATTACK)
	{
		weapon->SetN(nx);
		weapon->SetWeaponPosition(D3DXVECTOR3(x, y, 0), sitting);
	}
	weapon->Update(dt, coObjects);
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

	if (state == STAND_ATTACK || state == SIT_ATTACK)
	{
		weapon->Render();
	}

	standAttacking = !animations[state]->IsCompleted();
	sitAttacking = !animations[state]->IsCompleted();
	throwing = !animations[state]->IsCompleted();
	powering = !animations[state]->IsCompleted();
	stairUpping = !animations[state]->IsCompleted();
	deflecting = !animations[state]->IsCompleted();

	RenderBoundingBox();

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
		isStandOnStair = false;
	}

	else if (state == SIT_ATTACK)
	{
		sitting = true;
		isStandOnStair = false;
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

}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 13;
	top = y + 2;
	right = left + SIMON_BBOX_WIDTH;

	if (state != JUMP) bottom = top + SIMON_BBOX_HEIGHT;
	else bottom = top + SIMON_JUMPING_BBOX_HEIGHT;

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

#pragma endregion CheckState

void Simon::PositionCorrection(string prevState)
{
	float stair_x, stair_y;
	stairCollided->GetPosition(stair_x, stair_y);

	if (prevState == "")
	{
		if (state == STAIR_UP)
		{
			if (stairDirection == 1)
			{
				x = stair_x - 34.0f;
				y = stair_y - 31.0f;
			}
			else
			{
				x = stair_x + 5.0f;
				y = stair_y - 31.0f;
			}
		}
		else if (state == STAIR_DOWN)
		{
			if (stairDirection == 1)
			{
				x = stair_x - 10.0f;
				y = stair_y - 47.0f;
			}
			else
			{
				x = stair_x - 18.0f;
				y = stair_y - 47.0f;
			}
		}
	}
	else
	{
		if (state == STAIR_UP && prevState == STAIR_DOWN)
		{
			if (stairDirection == 1)
			{
				x -= 3.0f;
			}
			else
			{
				x += 3.0f;
			}
		}
		else if (state == STAIR_DOWN && prevState == STAIR_UP)
		{
			if (stairDirection == 1)
			{
				x += 3.0f;
			}
			else
			{
				x -= 3.0f;
			}
		}
	}
}

bool Simon::CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair)
{
	isMovingUp = false;
	isMovingDown = false;

	float simon_l, simon_t, simon_r, simon_b;
	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	// thu nhỏ vùng xét va chạm, chỉ xét va chạm với chân của Simon
	simon_t += 55;
	simon_b += 5;  // bottom +5 để xét cho va chạm với bậc thang đầu tiên khi bước xuống
	simon_l += 5;
	simon_r -= 5;

	for (UINT i = 0; i < listStair->size(); i++)
	{
		float stair_l, stair_t, stair_r, stair_b;
		listStair->at(i)->GetBoundingBox(stair_l, stair_t, stair_r, stair_b);

		if (Game::AABB(simon_l, simon_t, simon_r, simon_b, stair_l, stair_t, stair_r, stair_b))
		{
			//DebugOut(L"collision\n");

			if (listStair->at(i)->GetState() == STAIR_LEFT_UP) stairDirection = 1;
			else stairDirection = -1;

			stairCollided = listStair->at(i);

			// bậc thang ở dưới so với chân Simon->có thể di chuyển xuống.
			if (simon_b < stair_b) isMovingDown = true;
			if (stair_t - y <= 35) isMovingUp = true;

			// kiểm tra xem simon có thể di chuyển lên hay ko
			// vì mảng listStairs gồm các bậc thang liền kề nhau, nên chỉ cần kiểm tra 2 bậc là đủ.

			float upstair_x = -999, upstair_y = -999; // toạ độ của bậc thang liền kề ở phía trên (nếu có)

			if (i > 0)
			{
				listStair->at(i - 1)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT) // vì bậc nằm trên nên dy = -...
				{
					isMovingUp = true;
				}

				if (dx == GROUND_BBOX_WIDTH && dy == GROUND_BBOX_HEIGHT) // vì bậc nằm duoi nên dy = +...
				{
					isMovingDown = true;
				}
			}

			if (i < listStair->size() - 1)
			{
				listStair->at(i + 1)->GetPosition(upstair_x, upstair_y);

				float dx = abs(upstair_x - stair_l);
				float dy = upstair_y - stair_t;

				if (dx == GROUND_BBOX_WIDTH && dy == -GROUND_BBOX_HEIGHT)
				{
					isMovingUp = true;
				}

				if (dx == GROUND_BBOX_WIDTH && dy == GROUND_BBOX_HEIGHT) // vì bậc nằm duoi nên dy = +...
				{
					isMovingDown = true;
				}
			}

			return true; // collision between Simon and stairs
		}

	}

	return false;
}

bool Simon::CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem)
{
	float simon_l, simon_t, simon_r, simon_b;
	float item_l, item_t, item_r, item_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listItem->size(); i++)
	{
		if (listItem->at(i)->IsEnable() == false)
			continue;

		listItem->at(i)->GetBoundingBox(item_l, item_t, item_r, item_b);

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

bool Simon::CheckChangeScene(vector<LPCHANGESCENEOBJ>* listChangeScene)
{
	float simon_l, simon_t, simon_r, simon_b;
	float obj_l, obj_t, obj_r, obj_b;

	GetBoundingBox(simon_l, simon_t, simon_r, simon_b);

	for (UINT i = 0; i < listChangeScene->size(); i++)
	{
		listChangeScene->at(i)->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

		float t, nx, ny;
		Game::SweptAABB(simon_l, simon_t, simon_r, simon_b, dx, dy, obj_l, obj_t, obj_r, obj_b, t, nx, ny);
		bool collision = Game::AABB(simon_l, simon_t, simon_r, simon_b, obj_l, obj_t, obj_r, obj_b);

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
