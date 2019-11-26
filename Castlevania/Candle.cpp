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

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == DESTROY && isLastFame) 	//nếu render xong hết đốm lửa rồi thì set enable = false -> biến mất
	{
		this->isEnable = false;

		// Tạo một item theo id và thêm vào Objects
		if (!nameItem.empty())
		{
			// Tạo một item theo id và thêm vào Objects
			Items* item = new Items();
			item->isEnable = true;
			item->SetPosition(x, y);
			item->SetItem(nameItem);

			objects->push_back(item);
		}
	}
}


void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANDLE_BBOX_WIDTH;
	bottom = y + CANDLE_BBOX_HEIGHT;
}

void Candle::SetState(string state)
{
	GameObject::SetState(state);
}


void Candle::Render()
{
	animations[state]->Render(-1, x, y);
	this->isLastFame = animations[state]->IsCompleted();
}
