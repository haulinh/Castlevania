#pragma once

#include "Game.h"
#include "GameObject.h"
#include "TileMap.h"

#include "Candle.h"
#include "Ground.h"
#include "Simon.h"
#include "Weapon.h"
#include "Items.h"
#include "SubWeapon.h"
#include "Stair.h"



class SceneManager
{
	int IDScene;

	Game* game;
	Simon* simon;
	Candle* candle;
	Ground* ground;
	Items* item;
	Weapon* weapon;
	SubWeapon* subweapon;
	Stair* stair;

	TileMaps* tilemaps = TileMaps::GetInstance();
	Textures* textures = Textures::GetInstance();
	Sprites* sprites = Sprites::GetInstance();
	Animations* animations = Animations::GetInstance();

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listStairs;

public:
	SceneManager(Game* game, int idScene);
	~SceneManager();

	void LoadResources();		// load all sprites and tilemaps
	void LoadObjectsFromFile(LPCWSTR FilePath);		// load all objects (position, state, isenable) from file and save to vector Objects

	virtual void Update(DWORD dt);
	virtual void Render();

	void ChangeScene(int scene);
	Simon* GetSimon() { return this->simon; }
	SubWeapon* GetSubWeapon() { return this->subweapon; }
	int GetIDScene() { return this->IDScene; }
	vector<LPGAMEOBJECT>* GetListStairs() { return &(this->listStairs); }

};

