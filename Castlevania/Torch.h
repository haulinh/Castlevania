#pragma once
#include "GameObject.h"



class Torch :public GameObject
{
	int id;
public:
	Torch();
	void Update(int x, int y);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	
};

