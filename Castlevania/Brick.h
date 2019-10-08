#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32

class Brick : public GameObject
{
public:

	Brick();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
};