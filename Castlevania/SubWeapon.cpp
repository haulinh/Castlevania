#include "SubWeapon.h"
#include "LoadResourceFile.h"
#include "Candle.h"
#include "FireBall.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "VampireBat.h"
#include "FishMan.h"
#include "Ground.h"
#include "simon.h"

SubWeapon::SubWeapon()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSubWeapons = loadResourceFile->GetAnimations("resources\\sub_weapons\\sub_weapons_ani.xml");
	for each (string animation in animationsSubWeapons)
	{
		AddAnimation(animation);
	}
}

void SubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isHolyWaterShattered == true &&
		GetTickCount() - holyWaterShatteredCounter > WEAPONS_HOLY_WATER_TIME_EFFECT)
	{
		isHolyWaterShattered = false;
		holyWaterShatteredCounter = 0;
		this->isEnable = false;
		return;
	}

	GameObject::Update(dt);

	if (state == AXE_SUB)
	{
		vy += WEAPONS_AXE_GRAVITY * dt;
	}

	else if (state == HOLY_WATER_SUB)
	{
		vy += WEAPONS_HOLY_WATER_GRAVITY * dt;
	}

	else if (state == BOOMERANG_SUB)
	{
		if (nx > 0) vx -= WEAPONS_BOOMERANG_TURNBACK_SPEED;
		else vx += WEAPONS_BOOMERANG_TURNBACK_SPEED;
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		// kiểm tra va chạm với object
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* candle = dynamic_cast<Candle*>(e->obj);
				candle->SetState(CANDLE_DESTROYED);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
				{
					this->isEnable = false;
				}
			}
			else if (dynamic_cast<FireBall*>(e->obj))
			{
				FireBall* fireball = dynamic_cast<FireBall*>(e->obj);
				fireball->SetEnable(false);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
					this->isEnable = false;
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				Zombie* zombie = dynamic_cast<Zombie*>(e->obj);
				zombie->SetState(ZOMBIE_DESTROYED);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
					this->isEnable = false;
			}
			else if (dynamic_cast<BlackLeopard*>(e->obj))
			{
				BlackLeopard* blackLeopard = dynamic_cast<BlackLeopard*>(e->obj);
				blackLeopard->SetState(BLACK_LEOPARD_DESTROYED);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
					this->isEnable = false;
			}
			else if (dynamic_cast<VampireBat*>(e->obj))
			{
				VampireBat* vampirebat = dynamic_cast<VampireBat*>(e->obj);
				vampirebat->SetState(VAMPIRE_BAT_DESTROYED);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
					this->isEnable = false;
			}
			else if (dynamic_cast<FishMan*>(e->obj))
			{
				FishMan* fishman = dynamic_cast<FishMan*>(e->obj);
				fishman->SetState(FISHMAN_DESTROYED);

				if (state == DAGGER_SUB || state == AXE_SUB || state == BOOMERANG_SUB)
					this->isEnable = false;
			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (state == HOLY_WATER_SUB && e->ny == -1)
					SetState(HOLY_WATER_SHATTERED_SUB);
				
				x += dx;
				y += dy;
			}
			else if (dynamic_cast<Simon*>(e->obj))
			{
				if (state == BOOMERANG_SUB)
					this->isEnable = false;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SubWeapon::Render()
{
	animations[state]->Render(nx, x, y);
}

void SubWeapon::SetState(string state)
{
	GameObject::SetState(state);

	if (state == STOP_WATCH_SUB)
	{
	}

	else if (state == DAGGER_SUB)
	{
		if (nx > 0) vx = WEAPONS_DAGGER_SPEED;
		else vx = -WEAPONS_DAGGER_SPEED;
		vy = 0;
	}

	else if (state == AXE_SUB)
	{
		if (nx > 0) vx = WEAPONS_AXE_SPEED_X;
		else vx = -WEAPONS_AXE_SPEED_X;
		vy = -WEAPONS_AXE_SPEED_Y;
	}

	else if (state == HOLY_WATER_SUB)
	{
		vx = nx * WEAPONS_HOLY_WATER_SPEED_X;
		vy = -WEAPONS_HOLY_WATER_SPEED_Y;
	}

	else if (state == BOOMERANG_SUB)
	{
		vx = nx * WEAPONS_BOOMERANG_SPEED;
		vy = 0;
	}

	else if (state == HOLY_WATER_SHATTERED_SUB)
	{
		vx = 0;
		vy = 0;
		StartHolyWaterEffect();
	}
}

void SubWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (state == STOP_WATCH_SUB)
	{
		right = left + 26;
		bottom = top + 28;
	}

	else if (state == DAGGER_SUB)
	{
		right = left + WEAPONS_DAGGER_BBOX_WIDTH;
		bottom = top + WEAPONS_DAGGER_BBOX_HEIGHT;
	}

	else if (state == AXE_SUB)
	{
		right = left + WEAPONS_AXE_BBOX_WIDTH;
		bottom = top + WEAPONS_AXE_BBOX_HEIGHT;
	}

	else if (state == HOLY_WATER_SUB)
	{
		right = left + WEAPONS_HOLY_WATER_BBOX_WIDTH;
		bottom = top + WEAPONS_AXE_BBOX_HEIGHT;
	}

	else if (state == BOOMERANG_SUB)
	{
		right = left + WEAPONS_BOOMERANG_BBOX_WIDTH;
		bottom = top + WEAPONS_BOOMERANG_BBOX_HEIGHT;
	}
}



