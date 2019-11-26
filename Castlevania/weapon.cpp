#include "weapon.h"
#include "LoadResourceFile.h"
#include "Brick.h"
#include "Candle.h"
#include "Items.h"

Weapon::Weapon()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsWeapon = loadResourceFile->GetAnimations("resources\\weapons\\weapons_ani.xml");
	for each (string animation in animationsWeapon)
	{
		AddAnimation(animation);
	}

	state = MagicWhip;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->isLastFame)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<Candle*>(obj))
			{
				Candle* e = dynamic_cast<Candle*> (obj);

				if (this->AABBx(e) == true)
				{
					e->SetState(Destroy);
					e->isLastFame = false;
				}
			}
		}
	}
	this->isLastFame = false;
}

void Weapon::Render()
{
	animations[state]->Render(nx, x, y);
	this->isLastFame = this->animations[state]->IsCompleted();
}

void Weapon::Render(int id)
{
	animations[state]->Render(id, nx, x, y);
	this->isLastFame = this->animations[state]->GetCurrentFrame() == this->animations[state]->GetFrameSize() - 1;
	//if (isLastFame) RenderBoundingBox();
}

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + 15;
	bottom = top + WHIP_BBOX_HEIGHT;
	if (nx < 0)
	{
		if (state != LongChain)
			left = x + 50;
		else left = x + 20;
	}
	else if (nx > 0)
	{
		if (state != LongChain)
			left = (240 - 50) - WHIP_BBOX_WIDTH + x;
		else left = (240 - 20) - WHIP_BBOX_WIDTH + x;
	}

	right = left + WHIP_BBOX_WIDTH;
}

void Weapon::SetWeaponPosition(D3DXVECTOR3 simonPositon, bool sitting)
{
	simonPositon.x -= 90.0f;
	if (sitting)
		simonPositon.y += 15.0f;

	SetPosition(simonPositon.x, simonPositon.y);
}
