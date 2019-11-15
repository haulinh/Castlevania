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
}

void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nx < 0) vx = -ITEM_DAGGER_SPEED;
	else vx = ITEM_DAGGER_SPEED;

	GameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Candle*>(e->obj))
			{
				Candle* candle = dynamic_cast<Candle*>(e->obj);
					this->isEnable = false;
					candle->SetState(Destroy);
					candle->isLastFame = false;
			}
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

