#pragma once

#include <vector>
#include "GameObject.h"
typedef vector<LPGAMEOBJECT> listUnits;

class Grid
{
	int map_width;
	int map_height;

	int cell_width;
	int cell_height;

	int nums_col;
	int nums_row;

	int i = 0;
	vector<vector<listUnits>> cells;

public:
	Grid(int map_width, int map_height, int cell_width = DEFAULT_CELL_WIDTH, int cell_height = DEFAULT_CELL_HEIGHT);
	~Grid();

	void Add(LPGAMEOBJECT object);
	void Move(LPGAMEOBJECT object, float x, float y); // x, y là toạ độ mới của unit, vì vậy lúc này x, y của unit lưu vị trí cũ
	void Get(D3DXVECTOR2 camPosition, vector<LPGAMEOBJECT>& listUnits); // lấy tất cả các Unit* nằm trong vùng viewport để Update và Render

};

