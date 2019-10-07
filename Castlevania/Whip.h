#pragma once
#include "GameObject.h"
class Whip :
	public GameObject
{
public:
	Whip();

	void Update(int x, int y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

