#pragma once

#include "GameObject.h"

class VampireBat : public GameObject
{
	DWORD respawnTimeStart = 0;
	bool isRespawnWaiting = false;
	float velocityVariation = VAMPIRE_BAT_SPEED_VARIATION;

public:
	VampireBat();
	~VampireBat();

	virtual void Update(DWORD dt,  vector<LPGAMEOBJECT>* coObject = NULL);
	void Render();
	void SetState(string state);

	void StartRespawnTimeCounter() { isRespawnWaiting = true; respawnTimeStart = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetIsRespawnWaiting(bool x) { isRespawnWaiting = x; }
	bool IsRespawnWaiting() { return isRespawnWaiting; }
	bool IsAbleToActivate();
};

