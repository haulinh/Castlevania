#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <unordered_map>

#include "Sprites.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject; 
typedef GameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	CCollisionEvent(float t, float nx, float ny, LPGAMEOBJECT obj = NULL) { this->t = t; this->nx = nx; this->ny = ny; this->obj = obj; }

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};



class GameObject
{
public:

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;	 

	string state;

	bool isLastFame = false;
	bool isEnable;

	DWORD dt; 


	// vector<LPANIMATION> animations;
	unordered_map<string, LPANIMATION> animations;

public: 
	GameObject();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetN(int nx) { this->nx = nx; }

	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	string GetState() { return this->state; }


	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT*> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny);

	bool AABBx(LPGAMEOBJECT coO);

	void AddAnimation(string aniId);

	virtual void GetBoundingBox(float &left, float &top, float &width, float &height) = 0;
	void RenderBoundingBox();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT*> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(string state) { this->state = state; }

	~GameObject();
};

