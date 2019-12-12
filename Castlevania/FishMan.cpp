#include "FishMan.h"



FishMan::FishMan()
{
	SetState(FIREBALL);
}


FishMan::~FishMan()
{
}


void FishMan::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	DWORD now = GetTickCount();

	// Update bubbles
	if (isRenderingBubbles == true)
	{
		if (now - startTimeRenderBubbles <= 1000)
		{
			bubbles->Update(dt);
		}
		else
		{
			isRenderingBubbles = false;
			startTimeRenderBubbles = 0;
		}
	}

	// Update fishman
	if (state == FISHMAN_DESTROYED && animations[state]->IsOver(150) == true)
	{
		SetState(FISHMAN_INACTIVE);
		return;
	}
	else if (state == FISHMAN_HIT && animations[state]->IsOver(1000) == true)
	{
		SetState(FISHMAN_ACTIVE);
		return;
	}
	else if (state == FISHMAN_INACTIVE)
	{
		return;
	}

	GameObject::Update(dt);
	vy += FISHMAN_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0)
		{
			this->nx *= -1;
			this->vx *= -1;
		}

		if (ny != 0)
		{
			if (ny == -1.0f)
			{
				vy = 0;

				if (state == FISHMAN_JUMP) // jump xong chạm đất -> walk
				{
					SetState(FISHMAN_ACTIVE);

				}
			}
			else
			{
				y += dy;
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void FishMan::Render()
{
	// render bubbles
	if (isRenderingBubbles == true)
		bubbles->Render();

	// render fishman
	if (state != FISHMAN_INACTIVE)
		animations[state]->Render(nx, x, y);
}

void FishMan::SetState(string state)
{
	GameObject::SetState(state);

	if (state == FISHMAN_ACTIVE)
	{
		if (nx > 0) vx = FISHMAN_WALKING_SPEED_X;
		else vx = -FISHMAN_WALKING_SPEED_X;
	}
	else if (state == FISHMAN_JUMP)
	{
		x = entryPosition.x;
		y = entryPosition.y;
		vy = -FISHMAN_JUMP_SPEED_Y;
		isDroppedItem = false;
		isNeedToCreateBubbles = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
		startTime = GetTickCount();
		deltaTimeToShoot = rand() % 5000; // Random trong khoảng thời gian là 5s
		bubbles = new Bubbles(x, y + 32);
		isRenderingBubbles = true;
		startTimeRenderBubbles = GetTickCount();
	}
	else if (state == FISHMAN_DESTROYED)
	{
		vx = vy = 0;
		animations[state]->SetAniStartTime(GetTickCount());
	}
	else if (state == FISHMAN_INACTIVE)
	{
		vx = vy = 0;
		StartRespawnTimeCounter();
		if (isNeedToCreateBubbles == true)
		{
			bubbles = new Bubbles(x, y + 32);
			isRenderingBubbles = true;
			startTimeRenderBubbles = GetTickCount();
		}
	}
	else if (state == FISHMAN_HIT)
	{
		vx = vy = 0;
		animations[state]->SetAniStartTime(GetTickCount());
	}
}

void FishMan::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // 10,32
	top = y;
	right = left + FISHMAN_BBOX_WIDTH;
	bottom = top + FISHMAN_BBOX_HEIGHT;
}

void FishMan::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - FISHMAN_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + FISHMAN_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - FISHMAN_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y;
}

bool FishMan::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= FISHMAN_RESPAWN_TIME)
		return true;

	return false;
}

