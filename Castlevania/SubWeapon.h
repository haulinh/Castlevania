#pragma once
#include "GameObject.h"

class SubWeapon : public GameObject
{
public:
	SubWeapon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* objects, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(string state);

};

