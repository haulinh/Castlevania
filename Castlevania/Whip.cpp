#include "Whip.h"
#include "LoadResourceFile.h"
#include "Brick.h"
#include "Torch.h"

Whip::Whip()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsWhip= loadResourceFile->GetAnimations("resources\\whip\\whip_ani.xml");
	for each (string animation in animationsWhip)
	{
		AddAnimation(animation);
	}
}

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
 {
	if (this->isLastFame)
	{
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			GameObject* e = coObjects->at(i);
			if (dynamic_cast<Torch*>(e))
			{
				if (this->AABBx(e) == true)
				{
					DebugOut(L"Fuck");
					if (e->GetState() != torch_delete)
					{
						e->SetState(torch_delete);
					}
					coObjects->at(i)->SetDie(true);
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
	//if (isLastFame)
	//{
		RenderBoundingBox();
	//}

}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (nx == 1)
	{
		left = x + 140;
		top = y + 15;
		right = left + 85;
		bottom = top + 18;
	}
	else
	{
		left = x + 25;
		top = y + 15;
		right = left + 85;
		bottom = top + 18;
	}
}
