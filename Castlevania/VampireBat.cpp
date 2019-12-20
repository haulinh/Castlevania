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
	if (state == VAMPIRE_BAT_DESTROYED && isLastFame)
	{
		SetState(VAMPIRE_BAT_INACTIVE);
		return;
	}

	//if (stopMovement == true)
	//	return;

	GameObject::Update(dt);

	vy += velocityVariation;

	if (vy >= VAMPIRE_BAT_FLYING_SPEED_Y || vy <= -VAMPIRE_BAT_FLYING_SPEED_Y)
		velocityVariation *= -1;

	x += dx;
	y += dy;
}

void VampireBat::Render()
{
	if (state != VAMPIRE_BAT_INACTIVE)
	{
		animations[state]->Render(nx, x, y);
		isLastFame = animations[state]->IsCompleted();
	}
}

void VampireBat::SetState(string state)
{
	GameObject::SetState(state);

	if (state == VAMPIRE_BAT_ACTIVE)
	{
		if (nx > 0) vx = VAMPIRE_BAT_FLYING_SPEED_X;
		else vx = -VAMPIRE_BAT_FLYING_SPEED_X;
		vy = 0;
		isDroppedItem = false;
		respawnTimeStart = 0;
		isRespawnWaiting = false;
	}
	else if (state == VAMPIRE_BAT_DESTROYED)
	{
		vx = 0;
		vy = 0;
	}
	else if (state == VAMPIRE_BAT_INACTIVE)
	{
		x = entryPosition.x;
		y = entryPosition.y;
		vx = 0;
		vy = 0;
		isSettedPosition = false;
		StartRespawnTimeCounter();
	}
}

void VampireBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 11; // 10,32
	top = y + 11;
	right = left + VAMPIRE_BAT_BBOX_WIDTH;
	bottom = top + VAMPIRE_BAT_BBOX_HEIGHT;
}

void VampireBat::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = entryPosition.x - VAMPIRE_BAT_ACTIVE_BBOX_WIDTH;
	top = entryPosition.y - VAMPIRE_BAT_ACTIVE_BBOX_HEIGHT;
	right = entryPosition.x + VAMPIRE_BAT_ACTIVE_BBOX_WIDTH;
	bottom = entryPosition.y + VAMPIRE_BAT_ACTIVE_BBOX_HEIGHT;
}

bool VampireBat::IsAbleToActivate()
{
	DWORD now = NOW;

	if (isRespawnWaiting == true && now - respawnTimeStart >= VAMPIRE_BAT_RESPAWN_TIME)
		return true;

	return false;
}
