#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Ground.h"
#include "Candle.h"
#include "Items.h"

Simon::Simon() : GameObject() {

	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSimon = loadResourceFile->GetAnimations("resources\\simon\\simon_ani.xml");
	for each (string animation in animationsSimon)
	{
		AddAnimation(animation);
	}

	weapon = new Weapon();
	nameWeapon = HOLY_WATER_SUB;

	score = 0;
	item = -1;
	energy = 0;
	life = 3;
	HP = 10;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	if (state != STAIR_UP && state != STAIR_DOWN)
	{
		if (vy < -0.1f || vy > 0.1f)
			vy += SIMON_GRAVITY * dt;
		else vy += SIMON_GRAVITY_LOWER * dt;

	}

	// simple collision with border map
	if (x < 0) x = 0;

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

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->ny != 0)
				{
					vy = 0;
					jumping = false;
				}

				if (nx != 0 && state == STAIR_UP)
				{
					x -= nx * 0.1f;
				}

				if (state == STAIR_DOWN)
				{
					state = IDLE;
				}
			}

			// no collision of Simon and Candle
			if (dynamic_cast<Candle*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			else if (dynamic_cast<Items*>(e->obj))
			{
				//if (e->ny != 0) y = 0;
				e->obj->isEnable = false;

				string nameItem = e->obj->GetState();
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
				/*	 MONEY_BAG_BLUE:
					score += 400;

					 MONEY_BAG_WHITE:
					score += 700;

					 MONEY_BAG_FLASHING:
					score += 1000;

					 DOUBLE_SHOT:
					 TRIPLE_SHOT:
					item = idItem;

					 PORK_CHOP:
					HP += 2;

					 MAGIC_CRYSTAL:
					HP = 16;*/

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

	int alpha = 255;

	animations[state]->Render(nx, x, y, alpha);

	if (state == STAND_ATTACK || state == SIT_ATTACK)
	{
		weapon->Render();
	}

	standAttacking = !animations[state]->IsCompleted();
	sitAttacking = !animations[state]->IsCompleted();
	throwing = !animations[state]->IsCompleted();
	powering = !animations[state]->IsCompleted();
	stairUpping = !animations[state]->IsCompleted();

	//RenderBoundingBox();
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
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
		isStandOnStair = false;
	}

	else if (state == JUMP)
	{
		vy = -SIMON_JUMP_SPEED_Y;
		jumping = true;
		sitting = false;
		isStandOnStair = false;
	}

	else if (state == STAND_ATTACK)
	{
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

}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 10;
	top = y + 2;
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

//bool Simon::IsStairUpping()
//{
//	return (state == STAIR_UP && stairUpping);
//}

#pragma endregion CheckState

void Simon::PositionCorrection(string prevState)
{
	float stair_x, stair_y;
	stairCollided->GetPosition(stair_x, stair_y);

	if (prevState == "")
	{
		//DebugOut(L"FUCK");
		if (state == STAIR_UP)
		{
			if (stairDirection == 1)
			{
				x = stair_x - 34.0f;
				y = stair_y - 31.0f;
			}
			else
			{
				x = stair_x + 6.0f;
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
					return true;
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
					return true;
				}
			}

			// ko có bậc thang kế tiếp, tuy nhiên cần kiểm tra simon đã đi hết sprite thang hiện tại chưa
			// (một sprite là 32x32, gồm 2 bậc thang, mỗi lần simon chỉ đi lên 1 bậc)
			if (stair_t - y < 60)
			{
				isMovingUp = true;
				return true;
			}

			isMovingUp = false;
			return true; // collision between Simon and stairs
		}

	}

	isMovingUp = false;
	isMovingDown = false;

	return false;
}

void Simon::StandOnStair()
{
	vx = 0;
	vy = 0;
}
