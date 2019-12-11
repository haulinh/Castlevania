#include "Zombie.h"
#include "LoadResourceFile.h"



Zombie::Zombie()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsZombie = loadResourceFile->GetAnimations("resources\\zombie\\zombie_ani.xml");

	for each (string animation in animationsZombie)
	{
		AddAnimation(animation);
	}

	vector<string> animationsEffect = loadResourceFile->GetAnimations("resources\\effect\\effect_ani.xml");
	for each (string animation in animationsEffect)
	{
		AddAnimation(animation);
	}

	SetState(ZOMBIE);

	nx = -1;
	vx = -ZOMBIE_WALKING_SPEED;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == ZOMBIE_DESTROYED && isLastFame)
	{
		this->isEnable = false;
		return;
	}

	GameObject::Update(dt);

	x += dx;

	if (x <= 0)
	{
		x = 0;
		vx = ZOMBIE_WALKING_SPEED;
		nx = 1;
	}
	else if (x >= 500)
	{
		x = 500;
		vx = -ZOMBIE_WALKING_SPEED;
		nx = -1;
	}
}

void Zombie::Render()
{
	animations[state]->Render(nx, x, y);
	this->isLastFame = animations[state]->IsCompleted();
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ZOMBIE_BBOX_WIDTH;
	bottom = top + ZOMBIE_BBOX_HEIGHT;
}


