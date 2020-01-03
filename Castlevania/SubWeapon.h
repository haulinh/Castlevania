#pragma once
#include "GameObject.h"

class SubWeapon : public GameObject
{
	bool isHolyWaterShattered = false;
	int holyWaterShatteredCounter = 0;

	string targetTypeHit = ""; // Loại mục tiêu đánh trúng (dùng để kiểm tra máu của boss...)

	vector<vector<float>> sparkCoord; // vector lưu toạ độ để render spark khi subweapon trúng mục tiêu
	Animation* spark = Animations::GetInstance()->Get("spark");
	int startTimeRenderSpark = 0;
public:
	SubWeapon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();
	void RenderSpark();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(string state);

	void StartHolyWaterEffect() { isHolyWaterShattered = true; holyWaterShatteredCounter = GetTickCount(); }


	string GetTargetTypeHit() { return targetTypeHit; }
	void SetTargetTypeHit(string x) { targetTypeHit = x; }

	void GetCoordinateObject(LPGAMEOBJECT obj);			// Lấy toạ độ của object để lưu vào sparkC
};

