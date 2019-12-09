#pragma once

#include "GameObject.h"
#include "define.h"

class Door : public GameObject
{
public:
	Door();
	~Door();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObject = NULL) {}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);


};

