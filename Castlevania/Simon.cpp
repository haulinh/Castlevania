#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Brick.h"
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
	weapon->state = MagicWhip;
}

#pragma region Update 
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	vy += simon_gravity * dt;

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj))
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
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;

				e->obj->isEnable = false;

				if (e->obj->GetState() == CHAIN)
				{
					SetState(Power);
					if (weapon->GetState() == MagicWhip) weapon->SetState(ShortChain);
					else if (weapon->GetState() == ShortChain) weapon->SetState(LongChain);
				}

				else if (e->obj->GetState() == DAGGER)
				{
					vx = 0;
					isPowered = true;
				}
			}
			else
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (state == StandAttack || state == SitAttack)
	{
		weapon->SetN(nx);
		weapon->SetWeaponPosition(D3DXVECTOR3(x, y, 0), sitting);

		/*	if (animations[state]->GetCurrentFrame() == animations[state]->GetFrameSize() - 1)
			{
				for (UINT i = 0; i < coObjects->size(); i++)
				{
					LPGAMEOBJECT obj = coObjects->at(i);
					if (dynamic_cast<Candle*>(obj))
					{
						Candle* e = dynamic_cast<Candle*> (obj);

						if (weapon->AABBx(e) == true)
						{
							e->SetState(Destroy);
							e->isLastFame = false;
						}
					}
				}
			}*/
	}
	weapon->Update(dt, coObjects);
}
#pragma endregion Simon 


void Simon::Render()
{

	int alpha = 255;

	animations[state]->Render(nx, x, y, alpha);

	if (state == StandAttack || state == SitAttack)
	{

		int id = animations[state]->GetCurrentFrame();
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

	if (state == Walking)
	{
		if (nx > 0) vx = simon_walking_speed;
		else vx = -simon_walking_speed;
	}

	else if (state == Jump)
	{
		vy = -simon_jump_speed_y;
		jumping = true;
		sitting = false;
	}

	else if (state == StandAttack)
	{
	}

	else if (state == SitAttack)
	{
		sitting = true;
	}

	else if (state == Sit)
	{
		sitting = true;
		vx = 0;
		vy = 0;
	}

	else if (state == Idle)
	{
		sitting = false;
		vx = 0;
	}

	else if (state == Power)
	{
		vx = 0;
	}

}

bool Simon::IsJumping()
{
	return (state == Jump && jumping);
}

bool Simon::IsStandAttacking()
{
	return (state == StandAttack && standAttacking);
}

bool Simon::IsSitAttacking()
{
	return (state == SitAttack && sitAttacking);
}

bool Simon::IsThrowing()
{
	return (state == Throw && throwing);
}

bool Simon::IsPowering()
{
	return (state == Power && powering);
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	// sprite có kích thước là 60x66, bbox là 40x62
	left = x + 10;
	top = y + 2;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;

}

