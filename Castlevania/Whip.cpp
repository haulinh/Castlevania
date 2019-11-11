#include "Whip.h"
#include "LoadResourceFile.h"
#include "Brick.h"
#include "Candle.h"
#include "Items.h"

Whip::Whip()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsWhip= loadResourceFile->GetAnimations("resources\\whip\\whip_ani.xml");
	for each (string animation in animationsWhip)
	{
		AddAnimation(animation);
	}
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT*>* coObjects)
 {
	if (this->isLastFame)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT* obj = coObjects->at(i);
			if (dynamic_cast<Candle*>(*obj))
			{
				Candle* e = dynamic_cast<Candle*> (*obj);

				if (this->AABBx(e) == true)
				{
					DebugOut(L"Collision AABB\n");

					Items* items = new Items();
					items->isEnable = true;
					items->SetPosition(e->x, e->y);
					items->GeneratorRandom();

					*(obj) = items;
				}
			}
		}
	}
	this->isLastFame = false;
}

void Whip::Render()
{
	animations["whip"]->Render(nx, x, y);
	this->isLastFame = this->animations["whip"]->IsDoneCyle();
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		left = x + 50;
		right = left + WHIP_BBOX_WIDTH;
	}
	else if (nx > 0)
	{
		left = 190 - WHIP_BBOX_WIDTH + x;
		right = left + WHIP_BBOX_WIDTH;
	}
}
