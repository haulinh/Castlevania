#pragma once
#include "GameObject.h"
#include "debug.h"
class Weapon :
	public GameObject
{
public:
	Weapon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Render(int ID);

	void SetWeaponPosition(D3DXVECTOR3 simonPositon, bool sitting);
};

