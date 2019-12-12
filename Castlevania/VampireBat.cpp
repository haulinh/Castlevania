#include "VampireBat.h"



VampireBat::VampireBat()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsBat = loadResourceFile->GetAnimations("resources\\vampire_bat\\vampire_bat_ani.xml");

	for each (string animation in animationsBat)
	{
		AddAnimation(animation);
	}

	vector<string> animationsEffect = loadResourceFile->GetAnimations("resources\\effect\\effect_ani.xml");
	for each (string animation in animationsEffect)
	{
		AddAnimation(animation);
	}

	nx = 1;
}

VampireBat::~VampireBat()
{
}

void VampireBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	GameObject::Update(dt);

	vy += velocityVariation;

	if (vy >= VAMPIRE_BAT_FLYING_SPEED_Y || vy <= -VAMPIRE_BAT_FLYING_SPEED_Y)
		velocityVariation *= -1;

	x += dx;
	y += dy;
}

void VampireBat::Render()
{
	animations[state]->Render(nx, x, y);
}

void VampireBat::SetState(string state)
{
	GameObject::SetState(state);

	if (state == VAMPIRE_BAT_ACTIVE)
	{
		x = entryPosition.x;
		y = entryPosition.y;
		if (nx > 0) vx = VAMPIRE_BAT_FLYING_SPEED_X;
		else vx = -VAMPIRE_BAT_FLYING_SPEED_X;
		vy = 0;
		isDroppedItem = false;
		respawnTime_Start = 0;
		isRespawnWaiting = false;
	}
	else if (state == VAMPIRE_BAT_DESTROYED)
	{
		vx = 0;
	}
	else if (state == VAMPIRE_BAT_INACTIVE)
	{
		vx = 0;
		StartRespawnTimeCounter();
	}
}

void VampireBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // 10,32
	top = y;
	right = left + VAMPIRE_BAT_BBOX_WIDTH;
	bottom = top + VAMPIRE_BAT_BBOX_HEIGHT;
}

void VampireBat::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + VAMPIRE_BAT_ACTIVE_BBOX_WIDTH;
	bottom = top + VAMPIRE_BAT_ACTIVE_BBOX_HEIGHT;
}

bool VampireBat::IsAbleToActivate()
{
	DWORD now = GetTickCount();

	if (isRespawnWaiting == true && now - respawnTime_Start >= VAMPIRE_BAT_RESPAWN_TIME)
		return true;

	return false;
}
