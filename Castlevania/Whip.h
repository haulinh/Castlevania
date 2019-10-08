#pragma once
#include "GameObject.h"
#include "debug.h"
class Whip :
	public GameObject
{
public:
	Whip();

	void Update(float x, float y, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

