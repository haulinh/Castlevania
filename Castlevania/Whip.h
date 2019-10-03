#pragma once
#include "GameObject.h"
class Whip :
	public CGameObject
{
public:
	Whip();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

