#include "Candle.h"
#include "LoadResourceFile.h"
#include "define.h"

Candle::Candle()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();
	vector<string> animationsCandle = loadResourceFile->GetAnimations("resources\\Candle\\Candle_ani.xml");
	for each (string animation in animationsCandle)
	{
		AddAnimation(animation);
	}
}

void Candle::Update(int x, int y)
{
}


void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + Candle_BBOX_WIDTH;
	bottom = y + Candle_BBOX_HEIGHT;
}

void Candle::SetState(string state)
{
	GameObject::SetState(state);
}


void Candle::Render()
{
	if (state == "candle")
	{
		animations["candle"]->Render(x, y);
		RenderBoundingBox();
	}
}
