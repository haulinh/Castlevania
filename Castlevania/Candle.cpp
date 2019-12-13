#include "Candle.h"
#include "LoadResourceFile.h"
#include "define.h"
#include "debug.h"
#include "Items.h"

Candle::Candle()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsCandle = loadResourceFile->GetAnimations("resources\\candle\\candle_ani.xml");
	for each (string animation in animationsCandle)
	{
		AddAnimation(animation);
	}

	vector<string> animationsEffect = loadResourceFile->GetAnimations("resources\\effect\\effect_ani.xml");
	for each (string animation in animationsEffect)
	{
		AddAnimation(animation);
	}	

}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == CANDLE_DESTROY && isLastFame) 	//nếu render xong hết đốm lửa rồi thì set enable = false -> biến mất
	{
		this->isEnable = false;
	}
}


void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;

	if (state == LARGE_CANDLE)
	{
		right = left + LARGE_CANDLE_BBOX_WIDTH;
		bottom = top + LARGE_CANDLE_BBOX_HEIGHT;
	}

	else if (state == CANDLE)
	{
		right = left + CANDLE_BBOX_WIDTH;
		bottom = top + CANDLE_BBOX_HEIGHT;
	}
}

void Candle::SetState(string state)
{
	GameObject::SetState(state);
}


void Candle::Render()
{
	animations[state]->Render(1, x, y);
	this->isLastFame = animations[state]->IsCompleted();
}
