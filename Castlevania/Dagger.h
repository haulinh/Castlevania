#pragma once

#include "GameObject.h"
#include "Sprites.h"
#include "Textures.h"

class Dagger : public GameObject
{
public:
	Dagger();

	void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

