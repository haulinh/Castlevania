#include "Torch.h"
#include "LoadResourceFile.h"
#include "define.h"

Torch::Torch()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();
	vector<string> animationsTorch = loadResourceFile->GetAnimations("resources\\Torch\\Torch_ani.xml");
	for each (string animation in animationsTorch)
	{
		AddAnimation(animation);
	}
}

void Torch::Update(int x, int y)
{
}


void Torch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TORCH_BBOX_WIDTH;
	bottom = y + TORCH_BBOX_HEIGHT;
}

void Torch::SetState(int state)
{
	GameObject::SetState(state);
}


void Torch::Render()
{
	if (state == torch)
	{
		animations["torch"]->Render(x, y);
		RenderBoundingBox();
	}
}
