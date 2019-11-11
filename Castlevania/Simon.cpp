#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Brick.h"
#include "Candle.h"

Simon::Simon() : GameObject() {

	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSimon = loadResourceFile->GetAnimations("resources\\simon\\simon_ani.xml");
	for each (string animation in animationsSimon)
	{
		AddAnimation(animation);
	}

	whip = new Whip();
}

#pragma region Update 
void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);
	this->whip->Update(dt, coObjects);


	// Simple fall down
	vy += simon_gravity * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != Die)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Goomba 
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					jumping = false;
				}
			}
			if (dynamic_cast<Candle*>(e->obj))
			{
				x += dx;
				if (y <= 300 - 60 - 32)
				{
					y += dy;
				}
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	//whip->SetPosition(x - 94, y);
	//whip->SetN(nx);

}
#pragma endregion Simon 


void Simon::Render()
{

	int alpha = 255;
	if (untouchable) alpha = 128;

	animations[state]->Render(nx, x, y, alpha);
	attacking = !animations[state]->IsDoneCyle();
}

void Simon::SetState(string state)
{
	GameObject::SetState(state);

	if (state == Walking)
	{
		sitting = false;
		if (nx > 0) vx = simon_walking_speed;
		else vx = -simon_walking_speed;
		whip->SetN(nx);
	}
	else if (state == Jump)
	{
		vy = -simon_jump_speed_y;
		sitting = false;
		jumping = true;
	}

	else if (state == Attack)
	{
		this->whip->SetPosition(this->x - 90, this->y + 3);
		vx = 0;
	}

	else if (state == Sit)
	{
		sitting = true;
		vx = 0;
	}

	else if (state == Idle)
	{
		sitting = false;
		vx = 0;
	}

	else if (state == Die)
	{
		vy = -simon_die_deflect_speed;
	}
}

bool Simon::IsJumping()
{
	return (state == Jump && jumping);
}

bool Simon::IsAttacking()
{
	return (state == Attack && attacking);
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;

}

