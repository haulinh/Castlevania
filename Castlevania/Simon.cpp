#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"

#pragma region Update 
string ani;
void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

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

}
#pragma endregion Simon 


void CSimon::Render()
{

	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == SIMON_STATE_DIE)
		ani = "simon_ani_idle";
	else if (state == SIMON_STATE_SIT)
	{
		ani = "simon_ani_sitting";
		vx = 0;
	}
	else if (state == SIMON_STATE_JUMP)
	{
		ani = "simon_ani_sitting";
	}
	else if (state == SIMON_STATE_ATTACK)
	{
		ani = "simon_ani_attacking";
		//attacking = false;
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
		//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

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
		//attacking = true;
		vx = 0;
		break;
	case SIMON_STATE_SIT:
		sitting = true;
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

bool CSimon::IsAttacking()
{
	return (state == SIMON_STATE_ATTACK && attacking);
}


void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;

}

