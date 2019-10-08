#include "Whip.h"
#include "LoadResourceFile.h"
#include "Brick.h"

Whip::Whip()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsWhip= loadResourceFile->GetAnimations("resources\\whip\\whip_ani.xml");
	for each (string animation in animationsWhip)
	{
		AddAnimation(animation);
	}
}

void Whip::Update(float x, float y, vector<LPGAMEOBJECT>* coObjects)
 {
	SetPosition(x - 90, y);
	float objectLeft, objectTop, objectRight, objectBottom;
	GetBoundingBox(objectLeft, objectTop, objectRight, objectBottom);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Brick*>(coObjects->at(i))) 		{
			Brick* brick = dynamic_cast<Brick*>(coObjects->at(i));
			float otherLeft, otherTop, otherRight, otherBottom;
			brick->GetBoundingBox(otherLeft, otherTop, otherRight, otherBottom);
			if (isColliding(objectLeft, objectTop, objectRight, objectBottom, otherLeft, otherTop, otherRight, otherBottom))
			{
				brick->SetState(isEnable);
				DebugOut(L"Shitttttttttttttttttttttt\n");
				//coObjects->erase(coObjects->begin() + i);
			}
			else {
				brick->SetState(300);
			}
		}
	}
}

void Whip::Render()
{
	animations["whip"]->Render(nx, x, y);
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx == -1)
	{
		left = x + 50;
		top = y + 15;
		right = left + 55;
		bottom = top + 15;
	}
	else
	{
		left = (240 - 20) - 55 + x;
		top = y + 15;
		right = left + 40;
		bottom = top + 15;
	}
}
