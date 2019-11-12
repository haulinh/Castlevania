#include "Candle.h"
#include "LoadResourceFile.h"
#include "define.h"

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

	SetState(LargeCandle);
	this->isLastFame = false;
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == Destroy && isLastFame) 	//nếu render xong hết đốm lửa rồi thì set enable = false -> biến mất
	{
		this->isEnable = false;
	}
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
	animations[state]->Render(x, y);
	this->isLastFame = animations[state]->IsCompleted();
}
