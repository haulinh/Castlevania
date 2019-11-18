#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Textures.h"
#include "Debug.h"
#include "Sprites.h"

using namespace std;

class TileMap
{
	int mapWidth;			// chiều rộng của map
	int mapHeight;			// chiều cao của map

	int numsRow;			// số hàng của map
	int numsCol;			// số cột của map

	int tileWidth;			// chiều rộng một tile
	int tileHeight;		// chiều cao một tile

	LPCWSTR filePathTex;
	LPCWSTR filePathData;
	int ID;					// ID của tile map, dùng để tạo class quản lí TileMaps sau này ...

public:
	vector<vector<int>> mapData;	// ma trận lưu dữ liệu map

	Sprites* sprites;

	TileMap(int ID, LPCWSTR filePathTex, LPCWSTR filePathData, int mapWidth, int mapHeight, int tileWidth, int tileHeight);

	void LoadResources();	// load tất cả các block tile lưu vào sprites
	void LoadMapData();	// load ma trận dữ liệu map 
	void Draw(D3DXVECTOR2 camPosition);	// vẽ tất cả các tile nằm trong camera

	int GetMapWidth() { return mapWidth; }
};

typedef TileMap* LPTILEMAP;

class TileMaps
{
	static TileMaps* _instance;
	unordered_map<int, LPTILEMAP> tilemaps;

public:
	void Add(int ID, LPCWSTR filePath_tex, LPCWSTR filePath_data, int map_width, int map_height, int tile_width, int tile_height);
	LPTILEMAP Get(int ID) { return tilemaps[ID]; }

	static TileMaps* GetInstance();
};

