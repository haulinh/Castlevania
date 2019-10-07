#include "Whip.h"
#include "LoadResourceFile.h"

Whip::Whip()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsWhip= loadResourceFile->GetAnimations("resources\\whip\\whip_ani.xml");
	for each (string animation in animationsWhip)
	{
		AddAnimation(animation);
	}
}

void Whip::Update(int x, int y)
{
	SetPosition(x - 90, y);
}

void Whip::Render()
{
	animations["whip"]->Render(nx, x, y);
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 20;
	top = y + 18;
	right = left + 60 + 20;
	bottom = top + 18;
}
