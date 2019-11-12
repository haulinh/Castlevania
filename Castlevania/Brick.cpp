#include "Brick.h"
#include "LoadResourceFile.h"

Brick::Brick()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsGround = loadResourceFile->GetAnimations("resources\\ground\\ground_ani.xml");
	for each (string animation in animationsGround)
	{
		AddAnimation(animation);
	}
}

void Brick::Render()
{
	animations["ground_1"]->Render(x, y);
}

void Brick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void Brick::SetState(string state)
{
	GameObject::SetState(state);
}
