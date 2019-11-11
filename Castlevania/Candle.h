#pragma once
#include "GameObject.h"



class Candle :public GameObject
{
	int id;
public:
	Candle();
	void Update(int x, int y);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(string state);
	
};

