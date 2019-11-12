#include "Dagger.h"
#include "LoadResourceFile.h"
#include "define.h"
#include "Candle.h"
#include "Items.h"

Dagger::Dagger() : GameObject()
{

	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsItems = loadResourceFile->GetAnimations("resources\\items\\items_ani.xml");
	for each (string animation in animationsItems)
	{
		AddAnimation(animation);
	}

	vx = ITEM_DAGGER_SPEED;
	SetState(DAGGER);
}

void Dagger::Render()
{
	animations[state]->Render(-nx, x, y);
	RenderBoundingBox();
}

void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nx < 0) vx = -ITEM_DAGGER_SPEED;
	else vx = ITEM_DAGGER_SPEED;

	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occurred, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		//// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;

		// Collision logic with 
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Dagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

