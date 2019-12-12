#include "FireBall.h"



FireBall::FireBall()
{
	SetState(FIREBALL);
}


FireBall::~FireBall()
{
}


void FireBall::Update(DWORD dt,vector<LPGAMEOBJECT>* coObject)
{
	GameObject::Update(dt);
	x += dx;
}

void FireBall::Render()
{
	animations[state]->Render(1, nx, x, y);
}

void FireBall::SetState(string state)
{
	GameObject::SetState(state);

	if (state == FIREBALL)
	{
		if (nx == 1) vx = FIREBALL_SPEED;
		else vx = -FIREBALL_SPEED;
		vy = 0;
	}
}

void FireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

