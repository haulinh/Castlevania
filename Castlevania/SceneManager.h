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
#include "Door.h"
#include "Zombie.h"
#include "BlackLeopard.h"
#include "VampireBat.h"



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
	Door* door;
	Zombie* zombie;
	BlackLeopard* leopard;
	VampireBat* bat;

	ChangeSceneObject* changeScene;

	TileMaps* tilemaps = TileMaps::GetInstance();
	Textures* textures = Textures::GetInstance();
	Sprites* sprites = Sprites::GetInstance();
	Animations* animations = Animations::GetInstance();

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listCandles;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listGrounds;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listDoors;
	vector<LPGAMEOBJECT> listBlackLeopards;
	vector<LPGAMEOBJECT> listZombies;
	vector<LPGAMEOBJECT> listVampireBats;

	vector<LPCHANGESCENEOBJ> listChangeSceneObjs;

	bool isSetSimonAutoWalk = false;
	bool isMovingCamera = false;
	int countDxCamera = 0;

public:
	SceneManager(Game* game, int idScene);
	~SceneManager();

	void LoadResources();		// load all sprites and tilemaps
	void LoadObjectsFromFile(LPCWSTR FilePath);		// load all objects (position, state, isenable) from file and save to vector Objects
	void CreateListChangeSceneObjects();

	virtual void Update(DWORD dt);
	virtual void Render();

	void SetDropItems(LPGAMEOBJECT object);
	void SetInactivationByPosition();  // Nếu object ra khỏi toạ độ viewport thì set unable / inactive

	void ChangeScene(int scene);
	int GetIDScene() { return this->IDScene; }

	Simon* GetSimon() { return this->simon; }
	SubWeapon* GetSubWeapon() { return this->subweapon; }
	vector<LPGAMEOBJECT>* GetListStairs() { return &(this->listStairs); }

};

