#pragma once
#include "GameObject.h"
class Whip :
	public GameObject
{
public:
	Whip();

	void Update(int x, int y);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

