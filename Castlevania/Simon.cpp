#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#include "Goomba.h"
#include "Brick.h"

#pragma region Update 
Simon::Simon() : GameObject() {

	untouchable = 0;
	//whip = new Whip();
	//whip->SetN(nx);
	//whip->SetPosition(x - 94, y);


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
	vy += simon_gravity * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != simon_state_die)
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

	string ani;
	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == simon_state_die)
		ani = "simon_ani_idle";
	else if (state == simon_state_sit)
	{
		ani = "simon_ani_sitting";
	}
	else if (state == simon_state_jump)
	{
		ani = "simon_ani_sitting";
	}
	else if (state == simon_state_attack)
	{
		ani = "simon_ani_attacking";
		//attacking = false;	
	/*	whip->Render();*/
	}
	else if (state == simon_state_walking_left)
	{
		ani = "simon_ani_walking";
	}
	else if (state == simon_state_walking_right)
	{
		ani = "simon_ani_walking";
	}
	else if (state == simon_state_die)
		ani = "simon_ani_idle";
	else ani = "simon_ani_idle";

	animations[ani]->Render(nx, x, y, alpha);
	attacking = !animations[ani]->IsDoneCyle();
	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	GameObject::SetState(state);

	switch (state)
	{
	case simon_state_walking_right:
		sitting = false;
		vx = simon_walking_speed;
		nx = 1;
		break;
	case simon_state_walking_left:
		sitting = false;
		vx = -simon_walking_speed;
		nx = -1;
		break;
	case simon_state_jump:
		vy = -simon_jump_speed_y;
		sitting = false;
		jumping = true;
		break;
	case simon_state_attack:
		vx = 0;
		break;
	case simon_state_sit:
		sitting = true;
		vx = 0;
		break;
	case simon_state_idle:
		sitting = false;
		vx = 0;
		break;
	case simon_state_die:
		vy = -simon_die_deflect_speed;
		break;
	}
}

bool Simon::IsJumping()
{
	return (state == simon_state_jump && jumping);
}

bool Simon::IsAttacking()
{
	return (state == simon_state_attack && attacking);
}


void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BIG_BBOX_WIDTH;
	bottom = y + SIMON_BIG_BBOX_HEIGHT;

}

