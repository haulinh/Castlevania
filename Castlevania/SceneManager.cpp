﻿#include "SceneManager.h"


SceneManager::SceneManager(Game* game, int idScene)
{
	this->game = game;
	IDScene = idScene;
}

SceneManager::~SceneManager()
{
}

void SceneManager::LoadResources()
{

	LoadResourceFile* LoadResourceFile = LoadResourceFile::GetInstance();
	LoadResourceFile->LoadAllResource();


	simon = new Simon();

	candle = new Candle();

	candle = new Candle();

	item = new Items();

	weapon = new Weapon();

	subweapon = new SubWeapon();


	tilemaps->Add(SCENE_1, FILEPATH_TEX_MAP_SCENE_1, FILEPATH_DATA_MAP_SCENE_1, 1536, 320, 32, 32);
	tilemaps->Add(SCENE_2, FILEPATH_TEX_MAP_SCENE_2, FILEPATH_DATA_MAP_SCENE_2, 2880, 352, 32, 32);
}

void SceneManager::LoadObjectsFromFile(LPCWSTR FilePath)
{
	objects.clear();

	fstream fs;
	fs.open(FilePath, ios::in);
	if (fs.fail())
	{
		DebugOut(L"[INFO] Scene %d load data failed: file path = %s\n", IDScene, FilePath);
		fs.close();
		return;
	}

	string ID_Obj;
	float pos_x, pos_y;
	string state;
	bool isEnable;
	string nameItem;

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isEnable >> nameItem;

		if (ID_Obj == CANDLE)
		{
			candle = new Candle();
			candle->SetPosition(pos_x, pos_y);
			candle->SetState(state);
			candle->isEnable = isEnable;
			candle->SetIdItem(nameItem);
			objects.push_back(candle);
		}
		if (ID_Obj == BRICK)
		{
			ground = new Brick();
			ground->AddAnimation("brick");
			ground->SetPosition(pos_x, pos_y);
			ground->SetState(state);
			candle->isEnable = isEnable;
			objects.push_back(ground);
		}

	}
	fs.close();

	simon->SetPosition(0.0f, 220.0f);
	objects.push_back(simon);

	subweapon->SetEnable(false);
	objects.push_back(subweapon);

	//DebugOut(L"Objects size: %d \n", objects.size());
}

void SceneManager::Update(DWORD dt)
{
	float pos_x, pos_y;
	simon->GetPosition(pos_x, pos_y);

	//DebugOut(L"%f %f\n", pos_x, pos_y);

	if (IDScene == SCENE_1 && pos_x >= 1450.0f)
	{
		ChangeScene(SCENE_2);
		game->SetCamPos(0.0f, 0.0f);
		return;
	}

	for (int i = 0; i < objects.size(); i++)
	{

		if (objects[i]->isEnable == false)
			continue;

		vector<LPGAMEOBJECT> coObjects;

		if (dynamic_cast<Simon*>(objects[i]))
		{
			for (int j = 0; j < objects.size(); j++)
			{
				if (objects[j]->isEnable == false)
					continue;

				if (i != j) // thêm tất cả objects "ko phải là simon", dùng trong hàm update của simon 
					coObjects.push_back(objects[j]);
			}
		}
		else if (dynamic_cast<Items*>(objects[i]))
		{
			for (int j = 0; j < objects.size(); j++)
			{
				if (objects[i]->isEnable == false)
					continue;

				if (dynamic_cast<Brick*>(objects[j])) // thêm tất cả objects "là ground", dùng trong hàm update của item
				{
					coObjects.push_back(objects[j]);
				}
			}
		}
		else if (dynamic_cast<SubWeapon*>(objects[i]))
		{
			for (int j = 1; j < objects.size(); j++)
			{
				if (objects[j]->isEnable == false)
					continue;

				if (dynamic_cast<Brick*>(objects[j])) // thêm tất cả objects "là ground", dùng trong hàm update của subweapon
				{
					coObjects.push_back(objects[j]);
				}
			}
		}
		else
		{
			coObjects.push_back(objects[i]);
		}

		if (dynamic_cast<Candle*>(objects[i])) // ham update Candle co them para &object de them item vao object
		{
			Candle* e = dynamic_cast<Candle*>(objects[i]);
			e->Update(dt, &objects, &coObjects);
		}
		else objects[i]->Update(dt, &coObjects);
	}

	// render camera
	float cx, cy;
	simon->GetPosition(cx, cy);

	if (cx > SCREEN_WIDTH / 2 && cx + SCREEN_WIDTH / 2 < tilemaps->Get(IDScene)->GetMapWidth())
		game->SetCamPos(cx - SCREEN_WIDTH / 2, 0);
}

void SceneManager::Render()
{
	//tilemaps->Get(IDScene)->Draw(game->GetCamPos());

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isEnable == false)
			continue;

		objects[i]->Render();
		//objects[i]->RenderBoundingBox();
	}
}

void SceneManager::ChangeScene(int scene)
{
	IDScene = scene;

	switch (IDScene)
	{
	case SCENE_1:
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);
		break;
	case SCENE_2:
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_2);
		break;
	default:
		break;
	}
}