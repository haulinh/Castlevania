#pragma once
#include "GameObject.h"
#include "debug.h"
class Weapon :
	public GameObject
{
public:
	Weapon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetWeaponPosition(D3DXVECTOR3 simonPositon, bool sitting);
};

