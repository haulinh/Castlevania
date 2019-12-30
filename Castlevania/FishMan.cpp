﻿#include "FishMan.h"

FishMan::FishMan()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsFirstMan = loadResourceFile->GetAnimations("resources\\fish_man\\fish_man_ani.xml");

	for each (string animation in animationsFirstMan)
	{
		AddAnimation(animation);
	}

	vector<string> animationsEffect = loadResourceFile->GetAnimations("resources\\effect\\effect_ani.xml");
	for each (string animation in animationsEffect)
	{
		AddAnimation(animation);
	}
}


FishMan::~FishMan()
{
}


void FishMan::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	// Update bubbles
	if (isRenderingBubbles == true /*&& stopMovement == false*/)
	{
		DWORD now = GetTickCount();

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
	if (state == FISHMAN_DESTROYED && isLastFame)
	{
		SetState(FISHMAN_INACTIVE);
		return;
	}

	if (stopMovement == true)
		return;

	if (state == FISHMAN_HIT && animations[state]->IsOver(1000) == true)
	{
		nx = nxAfterShoot;
		SetState(FISHMAN_ACTIVE);
		return;
	}

	if (state == FISHMAN_INACTIVE)
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
	{
		animations[state]->Render(nx, x, y);
		isLastFame = animations[state]->IsCompleted();
	}
}

void FishMan::SetState(string state)
{
	GameObject::SetState(state);

	if (state == FISHMAN_ACTIVE)
	{
		if (nx > 0) vx = FISHMAN_WALKING_SPEED_X;
		else vx = -FISHMAN_WALKING_SPEED_X;
		lastTimeShoot = NOW;
		deltaTimeToShoot = 500 + rand() % 1000; // Random trong khoảng thời gian là 0.5 - 1s
	}
	else if (state == FISHMAN_JUMP)
	{
		vx = 0;
		vy = -FISHMAN_JUMP_SPEED_Y;
		isDroppedItem = false;
		isNeedToCreateBubbles = false;
		respawnTimeStart = 0;
		isRespawnWaiting = false;
		bubbles = new Bubbles(x, y + 32);
		isRenderingBubbles = true;
		startTimeRenderBubbles = NOW;
	}
	else if (state == FISHMAN_DESTROYED)
	{
		vx = vy = 0;
	}
	else if (state == FISHMAN_INACTIVE)
	{
		if (isNeedToCreateBubbles == true)
		{
			bubbles = new Bubbles(x, y + 32);
			isRenderingBubbles = true;
			startTimeRenderBubbles = NOW;
		}
		x = entryPosition.x;
		y = entryPosition.y;
		vx = vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
	}
	else if (state == FISHMAN_HIT)
	{
		vx = vy = 0;
	}
}

void FishMan::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // 10,32
	top = y + 2;
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
	DWORD now = NOW;

	if (isRespawnWaiting == true && now - respawnTimeStart >= FISHMAN_RESPAWN_TIME)
		return true;

	return false;
}

