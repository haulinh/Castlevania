#include "Dagger.h"
#include "LoadResourceFile.h"
#include "define.h"

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

void Dagger::Update(DWORD dt, vector<LPGAMEOBJECT*>* coObject)
{
	if (nx < 0) vx = -ITEM_DAGGER_SPEED;
	else vx = ITEM_DAGGER_SPEED;

	GameObject::Update(dt);
	x += dx;
}

void Dagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + DAGGER_BBOX_WIDTH;
	bottom = top + DAGGER_BBOX_HEIGHT;
}

