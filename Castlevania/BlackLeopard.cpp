#include "BlackLeopard.h"

BlackLeopard::BlackLeopard()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsBlackLeopard = loadResourceFile->GetAnimations("resources\\black_leopard\\black_leopard_ani.xml");

	for each (string animation in animationsBlackLeopard)
	{
		AddAnimation(animation);
	}

	vector<string> animationsEffect = loadResourceFile->GetAnimations("resources\\effect\\effect_ani.xml");
	for each (string animation in animationsEffect)
	{
		AddAnimation(animation);
	}
	nx = -1;
}

void BlackLeopard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	DWORD now = GetTickCount();

	if (state == BLACK_LEOPARD_DESTROYED && isLastFame)
	{
		SetState(BLACK_LEOPARD_INACTIVE);
		return;
	}

	vy += BLACK_LEOPARD_GRAVITY * dt;
	GameObject::Update(dt);


	// Check collision between zombie and ground (jumping on ground)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObject, coEvents);

	//DebugOut(L"%d ", coEvents.size());

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (state == BLACK_LEOPARD_ACTIVE && isJumping == false) // không va chạm với ground và chưa nhảy -> nhảy
		{
			isJumping = true;
			SetState(BLACK_LEOPARD_JUMP);
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += dx;
		y += min_ty * dy + ny * 0.1f;

		if (ny != 0)
		{
			vy = 0;

			if (state == BLACK_LEOPARD_JUMP)
			{
				this->nx = 1;
				SetState(BLACK_LEOPARD_ACTIVE);
			}
		}
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BlackLeopard::Render()
{
	if (state != BLACK_LEOPARD_INACTIVE)
	{
		if (isRespawnWaiting == false)
		{
			animations[state]->Render(nx, x, y);
			this->isLastFame = animations[state]->IsCompleted();
		}
	}
}

void BlackLeopard::SetState(string state)
{
	GameObject::SetState(state);

	if (state == BLACK_LEOPARD_ACTIVE)
	{
		if (nx > 0) vx = BLACK_LEOPARD_RUNNING_SPEED_X;
		else vx = -BLACK_LEOPARD_RUNNING_SPEED_X;
		vy = BLACK_LEOPARD_RUNNING_SPEED_Y;
		//isDroppedItem = false;
		respawnTimeStart = 0;
		isRespawnWaiting = false;
	}
	else if (state == BLACK_LEOPARD_DESTROYED)
	{
		vx = 0;
	}
	else if (state == BLACK_LEOPARD_INACTIVE)
	{
		vx = 0;
		vy = 0;
		StartRespawnTimeCounter();
	}
	else if (state == BLACK_LEOPARD_IDLE)
	{
		vx = 0;
	}
	else if (state == BLACK_LEOPARD_JUMP)
	{
		vy = -BLACK_LEOPARD_RUNNING_SPEED_Y;
	}
}

void BlackLeopard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 27;  // (10/64)
	top = y;
	right = left + BLACK_LEOPARD_BBOX_WIDTH;
	bottom = top + BLACK_LEOPARD_BBOX_HEIGHT;
}

void BlackLeopard::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - BLACK_LEOPARD_ACTIVE_BBOX_WIDTH;
	right = entryPosition.x + BLACK_LEOPARD_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - BLACK_LEOPARD_ACTIVE_BBOX_HEIGHT;
	bottom = entryPosition.y + BLACK_LEOPARD_ACTIVE_BBOX_HEIGHT;
}

bool BlackLeopard::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTimeStart >= BLACK_LEOPARD_RESPAWN_TIME)
		return true;

	return false;
}


