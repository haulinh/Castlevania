#pragma once
#include "GameObject.h"
#include "debug.h"

#define BRICK_BBOX_WIDTH  32
#define BRICK_BBOX_HEIGHT 32

class Items : public GameObject
{
	DWORD timeAppear;   // thời gian bắt đầu xuất hiện của item, dùng để xét cho item tự huỷ
public:

	Items();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(string state);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);

	void GeneratorRandom();
	void SetItem(string nameItem);
};