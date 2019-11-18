#include "Items.h"
#include "LoadResourceFile.h"

Items::Items()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsItems = loadResourceFile->GetAnimations("resources\\items\\items_ani.xml");
	for each (string animation in animationsItems)
	{
		AddAnimation(animation);
	}

	timeAppear = -1;
	vy = ITEM_FALLING_SPEED;
}

void Items::Render()
{
	animations[state]->Render(-1, x, y);
}

void Items::SetState(string state)
{
	GameObject::SetState(state);
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (timeAppear == -1)
	{
		timeAppear = GetTickCount();
	}
	else
	{
		DWORD now = GetTickCount();

		if (now - timeAppear > ITEM_TIME_DESTROYED)
		{
			isEnable = false;
			return;
		}
	}


	GameObject::Update(dt);

	// Check collision between item and ground (falling on ground)
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

		y += min_ty * dy + ny * 0.4f;
		if (ny != 0) vy = 0;
	}

	// clean up collision events
	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Items::GeneratorRandom()
{
	vector<string> listState = { "LargeHeart", "Chain", "Dagger" };
	int randomState = rand() % 3;
	state = listState[randomState];
}

void Items::SetItem(int id)
{
	vector<string> listState = { "LargeHeart", "Chain", "Dagger" };
	state = listState[id];
}

void Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;


	if (state == LARGE_HEART)
	{
		right = left + LARGE_HEART_BBOX_WIDTH;
		bottom = top + LARGE_HEART_BBOX_HEIGHT;

	}
	else if (state == CHAIN)
	{
		right = left + CHAIN_BBOX_WIDTH;
		bottom = top + CHAIN_BBOX_HEIGHT;
	}
	else if (state == DAGGER)
	{
		right = left + DAGGER_BBOX_WIDTH;
		bottom = top + DAGGER_BBOX_HEIGHT;
	}
}

