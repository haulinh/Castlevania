#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"

#pragma region Update 
string ani;

Simon::Simon() : GameObject() {



	level = SIMON_LEVEL_BIG;
	untouchable = 0;
	whip = new Whip();
	whip->SetN(nx);
	whip->SetPosition(x - 94, y);


	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsSimon = loadResourceFile->GetAnimations("resources\\simon\\simon_ani.xml");
	for each (string animation in animationsSimon)
	{
		AddAnimation(animation);
	}
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	GameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
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

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					jumping = false;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	whip->SetPosition(x - 94, y);
	whip->SetN(nx);

}
#pragma endregion Simon 


void Simon::Render()
{

	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == SIMON_STATE_DIE)
		ani = "simon_ani_idle";
	else if (state == SIMON_STATE_SIT)
	{
		ani = "simon_ani_sitting";
	}
	else if (state == SIMON_STATE_JUMP)
	{
		ani = "simon_ani_sitting";
	}
	else if (state == SIMON_STATE_ATTACK)
	{
		ani = "simon_ani_attacking";
		//attacking = false;	
		whip->Render();
	}
	else if (state == SIMON_STATE_WALKING_LEFT)
	{
		ani = "simon_ani_walking";
	}
	else if (state == SIMON_STATE_WALKING_RIGHT)
	{
		ani = "simon_ani_walking";
	}
	else if (state == SIMON_STATE_DIE)
		ani = "simon_ani_idle";
	else ani = "simon_ani_idle";

	animations[ani]->Render(nx, x, y, alpha);
	attacking = !animations[ani]->IsDoneCyle();
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		sitting = false;
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		sitting = false;
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		sitting = false;
		jumping = true;
		break;
	case SIMON_STATE_ATTACK:
		vx = 0;
		break;
	case SIMON_STATE_SIT:
		sitting = true;
		vx = 0;
		break;
	case SIMON_STATE_IDLE:
		sitting = false;
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	}
}

bool Simon::IsJumping()
{
	return (state == SIMON_STATE_JUMP && jumping);
}

bool Simon::IsAttacking()
{
	return (state == SIMON_STATE_ATTACK && attacking);
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;

}

