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
	SetPosition(x, y);
}

void Whip::Render()
{
	animations["whip"]->Render(nx, x, y);
}

void Whip::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
