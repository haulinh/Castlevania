#include "SubWeapon.h"
#include "LoadResourceFile.h"
#include "Candle.h"

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
	GameObject::Update(dt);

	if (state == AXE_SUB)
	{
		vy += 0.02f;
	}

	else if (state == HOLY_WATER_SUB)
	{
		vy += 0.02f;
	}

	else if (state == BOOMERANG_SUB)
	{
		vx += (-nx * 0.01f);
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
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* candle = dynamic_cast<Candle*>(e->obj);
				this->isEnable = false;
				candle->SetState(DESTROY);
				candle->isLastFame = false;
			}
		}

		/*	if (state == HOLY_WATER_SUB && ny != 0)
			{
				SetEnable(false);
			}
			else if (state == AXE_SUB)
			{
				if (y > 350.0f) SetEnable(false);
			}*/
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
		vx = nx * WEAPONS_DAGGER_SPEED;
		vy = 0;
	}

	else if (state == AXE_SUB)
	{
		vx = nx * WEAPONS_AXE_SPEED_X;
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
		right = left + 32;
		bottom = top + 18;
	}

	else if (state == AXE_SUB)
	{
		right = left + 30;
		bottom = top + 28;
	}

	else if (state == HOLY_WATER_SUB)
	{
		right = left + 32;
		bottom = top + 32;
	}

	else if (state == BOOMERANG_SUB)
	{
		right = left + 30;
		bottom = top + 28;
	}

}



