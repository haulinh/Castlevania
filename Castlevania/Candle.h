#pragma once
#include "GameObject.h"



class Candle :public GameObject
{
	int id;
public:
	Candle();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(string state);
	
};

