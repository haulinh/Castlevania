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

	vy += SIMON_GRAVITY * dt;

	// simple collision with border map
	if (x < 0) x = 0;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
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
			}

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

	//RenderBoundingBox();
}

void Simon::SetState(string state)
{
	GameObject::SetState(state);

	if (state == WALK)
	{
		if (nx > 0) vx = SIMON_WALKING_SPEED;
		else vx = -SIMON_WALKING_SPEED;
	}

	else if (state == JUMP)
	{
		vy = -SIMON_JUMP_SPEED_Y;
		jumping = true;
		sitting = false;
	}

	else if (state == STAND_ATTACK)
	{
	}

	else if (state == SIT_ATTACK)
	{
		sitting = true;
	}

	else if (state == SIT)
	{
		sitting = true;
		vx = 0;
		vy = 0;
	}

	else if (state == IDLE)
	{
		sitting = false;
		vx = 0;
	}

	else if (state == POWER)
	{
		vx = 0;
	}

}

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


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 10;
	top = y + 2;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;

}

