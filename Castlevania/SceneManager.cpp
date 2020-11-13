﻿#include "SceneManager.h"
#include "Door.h"


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

	door = new Door();

	zombie = new Zombie();

	leopard = new BlackLeopard();

	fishman = new FishMan();

	fireball = new FireBall();

	bubble = new Bubble();

	bat = new VampireBat();

	tilemaps->Add(SCENE_1, FILEPATH_TEX_MAP_SCENE_1, FILEPATH_DATA_MAP_SCENE_1, 1536, 320, 32, 32);
	tilemaps->Add(SCENE_2, FILEPATH_TEX_MAP_SCENE_2, FILEPATH_DATA_MAP_SCENE_2, 5632, 352, 32, 32);
	tilemaps->Add(SCENE_3, FILEPATH_TEX_MAP_SCENE_3, FILEPATH_DATA_MAP_SCENE_3, 1024, 352, 32, 32);
}

void SceneManager::LoadObjectsFromFile(LPCWSTR FilePath)
{
	objects.clear();
	listCandles.clear();
	listStairs.clear();
	listGrounds.clear();
	listItems.clear();
	listDoors.clear();
	listZombies.clear();

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
	string typeStair;

	while (!fs.eof())
	{
		fs >> ID_Obj >> pos_x >> pos_y >> state >> isEnable >> nameItem >> typeStair;

		if (ID_Obj == CANDLE)
		{
			candle = new Candle();
			candle->SetPosition(pos_x, pos_y);
			candle->SetState(state);
			candle->isEnable = isEnable;
			candle->SetIdItem(nameItem);
			listCandles.push_back(candle);
			objects.push_back(candle);
		}
		else if (ID_Obj == GROUND)
		{
			ground = new Ground();
			ground->SetPosition(pos_x, pos_y);
			ground->SetState(state);
			ground->isEnable = isEnable;
			listGrounds.push_back(ground);
			objects.push_back(ground);
		}
		else if (ID_Obj == STAIR)
		{
			stair = new Stair();
			stair->SetPosition(pos_x, pos_y);
			stair->SetState(state);
			stair->SetEnable(isEnable);
			stair->SetType(typeStair);
			listStairs.push_back(stair);
			objects.push_back(stair);
		}
		else if (ID_Obj == DOOR)
		{
			door = new Door();
			door->SetPosition(pos_x, pos_y);
			door->SetState(state);
			door->SetEnable(isEnable);
			listDoors.push_back(door);
			objects.push_back(door);
		}
		else if (ID_Obj == ZOMBIE)
		{
			zombie = new Zombie();
			zombie->SetEntryPosition(pos_x, pos_y);
			zombie->SetState(ZOMBIE_INACTIVE);
			zombie->SetEnable(isEnable);
			listZombies.push_back(zombie);
			objects.push_back(zombie);
		}
		else if (ID_Obj == BLACK_LEOPARD)
		{
			leopard = new BlackLeopard();
			leopard->SetEntryPosition(pos_x, pos_y);
			leopard->SetPosition(pos_x, pos_y);
			leopard->SetState(state);
			leopard->SetIsRespawnWaiting(false);
			leopard->SetEnable(true);
			listBlackLeopards.push_back(leopard);
			objects.push_back(leopard);
		}
		else if (ID_Obj == VAMPIRE_BAT)
		{
			bat = new VampireBat();
			bat->SetEntryPosition(pos_x, pos_y);
			bat->SetState(VAMPIRE_BAT_INACTIVE);
			bat->SetEnable(true);
			listVampireBats.push_back(bat);
			objects.push_back(bat);
		}

	}
	fs.close();

	//// 
	//bat = new VampireBat();
	//bat->SetEntryPosition(0.0f, 150.0f);
	//bat->SetState(VAMPIRE_BAT_ACTIVE);
	//bat->SetEnable(isEnable);
	//listVampireBats.push_back(bat);
	//objects.push_back(bat);

	objects.push_back(simon);

	subweapon->SetEnable(false);
	objects.push_back(subweapon);
}

void SceneManager::CreateListChangeSceneObjects()
{
	listChangeSceneObjs.clear();

	switch (IDScene)
	{
	case SCENE_1:
		changeScene = new ChangeSceneObject(1440, 336, SCENE_2);
		listChangeSceneObjs.push_back(changeScene);
		break;
	case SCENE_2:
		changeScene = new ChangeSceneObject(3168, 432, SCENE_3);
		listChangeSceneObjs.push_back(changeScene);
		changeScene = new ChangeSceneObject(3808, 432, SCENE_3);
		listChangeSceneObjs.push_back(changeScene);
		break;
	case SCENE_3:
		changeScene = new ChangeSceneObject(96, 80, SCENE_2);
		listChangeSceneObjs.push_back(changeScene);
		changeScene = new ChangeSceneObject(736, 80, SCENE_2);
		listChangeSceneObjs.push_back(changeScene);
		break;
	default:
		break;
	}
}

void SceneManager::Update(DWORD dt)
{
	// Cập nhật trạng thái Simon đi qua cửa:
	// Di chuyển Camera -> Mở cửa -> AutoWalk -> Di chuyển Camera

	if (simon->isWalkThroughDoor == true)
	{
		isMovingCamera = true;
		countDxCamera = 0;

		simon->isWalkThroughDoor = false;
	}

	if (isMovingCamera == true)
	{
		if (countDxCamera < 224)	// Di chuyển camera một đoạn 224
		{
			D3DXVECTOR3 cam = game->GetCamPos();
			game->SetCamPos(cam.x + 2, cam.y);
			countDxCamera += 2;

			return;
		}

		if (isSetSimonAutoWalk == false)  // AutoWalk
		{
			isSetSimonAutoWalk = true;

			simon->SetState(WALK);
			simon->vy = 0;
			simon->AutoWalk(120, IDLE, 1);
		}
		else
		{
			if (simon->IsAutoWalk() == false)
			{
				if (countDxCamera < 480) // Di chuyển camera thêm một đoạn nữa
				{
					D3DXVECTOR3 cam = game->GetCamPos();
					game->SetCamPos(cam.x + 2, cam.y);
					countDxCamera += 2;

					return;
				}
				else
				{
					isMovingCamera = false;
					isSetSimonAutoWalk = false;
					countDxCamera = 0;

					tilemaps->Get(IDScene)->index += 1;  // tăng giới hạn của tilemap
				}
			}
		}
	}

	// Khi Simon va chạm với ChangScene objects, tiến hành thay đổi, cập nhật trạng thái
	if (simon->GetChangeScene() != -1)
	{
		if (IDScene == SCENE_1 ||
			(IDScene == SCENE_2 && simon->GetState() == STAIR_DOWN) ||
			(IDScene == SCENE_3 && simon->GetState() == STAIR_UP))
		{
			bool isNeedToUpdatePosition = false;

			if (IDScene != SCENE_1) isNeedToUpdatePosition = true;

			float x, y;
			simon->GetPosition(x, y);

			//DebugOut(L"%f %f \n", x, y);

			IDScene = simon->GetChangeScene();
			ChangeScene(IDScene);

			if (isNeedToUpdatePosition == true)
			{
				switch (IDScene)
				{
				case SCENE_3:
					if (x < 3200.0f) simon->SetPosition(93.0f, 48.0f);
					else simon->SetPosition(733.0f, 48.0f);

					tilemaps->Get(IDScene)->index = 0;
					game->SetCamPos(0.0f, 0.0f);
					break;
				case SCENE_2:
					if (x < 320.0f) simon->SetPosition(3136.0f, 335.0f);
					else simon->SetPosition(3776.0f, 335.0f);

					simon->SetState(IDLE);
					tilemaps->Get(IDScene)->index = 1;
					game->SetCamPos(3056.0f, 0.0f);
					break;
				default:
					break;
				}
			}

			simon->SetChangeScene(-1);
		}
		else
		{
			simon->SetChangeScene(-1);
		}
	}

	// Cập nhật trạng thái hoạt động khi object đi ra khỏi viewport
	SetInactivationByPosition();

	for (int i = 0; i < objects.size(); i++)
	{

		LPGAMEOBJECT object = objects[i];

		if (object->IsEnable() == false)
		{
			if (dynamic_cast<Candle*>(object))
			{
				if (object->nameItem != "")
				{
					// Tạo một item theo id và thêm vào Objects

					item = new Items();
					item->SetEnable(true);
					item->SetPosition(object->x, object->y);
					item->SetItem(object->nameItem);

					listItems.push_back(item);
					objects.push_back(item);
				}
			}

			objects.erase(objects.begin() + i);
			i--;

			continue;
		}

		vector<LPGAMEOBJECT> coObjects;
		if (dynamic_cast<Simon*>(object))
		{
			for (auto candle : listCandles)
			{
				if (candle->IsEnable() == false)
					continue;

				coObjects.push_back(candle);
			}

			for (auto ground : listGrounds)
			{
				if (ground->IsEnable() == false)
					continue;

				coObjects.push_back(ground);
			}

			for (auto zombie : listZombies)
			{
				if (zombie->GetState() == ZOMBIE_INACTIVE)
					continue;

				coObjects.push_back(zombie);
			}

			for (auto door : listDoors)
			{
				if (door->IsEnable() == false)
					continue;

				coObjects.push_back(door);
			}

			for (auto leopard : listBlackLeopards)
			{
				if (leopard->GetState() == BLACK_LEOPARD_INACTIVE)
					continue;

				coObjects.push_back(leopard);
			}

			for (auto bat : listVampireBats)
			{
				if (bat->GetState() == VAMPIRE_BAT_INACTIVE)
					continue;

				coObjects.push_back(bat);
			}

			simon->Update(dt, &coObjects);
			simon->CheckCollisionWithItem(&listItems);
			simon->CheckChangeScene(&listChangeSceneObjs);
			//simon->CheckCollisionWithEnemyActiveArea(&listZombies);
			simon->CheckCollisionWithEnemyActiveArea(&listVampireBats);
			simon->CheckCollisionWithEnemyActiveArea(&listBlackLeopards);

		}
		else if (dynamic_cast<Items*>(objects[i]))
		{
			object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<Zombie*>(objects[i]))
		{
			object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<BlackLeopard*>(objects[i]))
		{
			object->Update(dt, &listGrounds);
		}
		else if (dynamic_cast<VampireBat*>(object))
		{
			if (object->GetState() != VAMPIRE_BAT_INACTIVE)
			{
				bat = dynamic_cast<VampireBat*>(object);

				if (bat->IsSettedPosition() == false)
				{
					bat->SetIsSettedPosition(true);

					// set vị trí cho dơi luôn 
					// dơi bay ngang tầm simon, từ phía cuối của 2 đầu màn hình)
					float bx, by;

					by = simon->y;

					if (bat->GetN() == -1) bx = game->GetCamPos().x + SCREEN_WIDTH - VAMPIRE_BAT_BBOX_WIDTH;
					else bx = game->GetCamPos().x;

					bat->SetPosition(bx, by);
				}

				bat->Update(dt);
			}

		}
		else if (dynamic_cast<SubWeapon*>(objects[i]))
		{
			//for (int j = 0; j < objects.size(); j++)
			//{
			//	if (objects[j]->isEnable == false)
			//		continue;

			//	if (dynamic_cast<Ground*>(objects[j])) // thêm tất cả objects "là ground", dùng trong hàm update của subweapon
			//	{
			//		coObjects.push_back(objects[j]);
			//	}

			//	else if (dynamic_cast<Candle*>(objects[j])) // thêm tất cả objects "là ground", dùng trong hàm update của subweapon
			//	{
			//		coObjects.push_back(objects[j]);
			//	}
			//}
		}
		else objects[i]->Update(dt, &coObjects);
	}

	// update camera

	TileMap* map = tilemaps->Get(IDScene);
	int min_col = map->min_max_col_to_draw[map->index][0];
	int max_col = map->min_max_col_to_draw[map->index][1];

	if (simon->x > SCREEN_WIDTH / 2 &&
		simon->x + SCREEN_WIDTH / 2 < tilemaps->Get(IDScene)->GetMapWidth())
	{
		if (simon->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			simon->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{
			game->SetCamPos(simon->x - SCREEN_WIDTH / 2, 0);
		}
	}
}

void SceneManager::Render()
{
	tilemaps->Get(IDScene)->Draw(game->GetCamPos());

	for (auto candle : listCandles)
	{
		if (candle->IsEnable() == false)
			continue;

		candle->Render();
		//candle->RenderBoundingBox();
	}

	//for (auto stair : listStairs)
	//{
	//	stair->RenderBoundingBox();
	//}

	for (auto stair : listStairs)
	{
		stair->RenderBoundingBox();
		//candle->RenderBoundingBox();
	}

	for (auto item : listItems)
	{
		if (item->IsEnable() == false)
			continue;

		item->Render();
		//item->RenderBoundingBox();
	}

	simon->Render();
	//simon->RenderBoundingBox();

	for (auto door : listDoors)
	{
		if (door->IsEnable() == false)
			continue;

		door->Render();
		door->RenderBoundingBox();
	}

	for (auto zombie : listZombies)
	{
		//zombie->RenderActiveBoundingBox();
		if (zombie->GetState() == ZOMBIE_INACTIVE)
			continue;

		zombie->Render();
		//zombie->RenderBoundingBox();
	}

	for (auto leopard : listBlackLeopards)
	{
		//zombie->RenderActiveBoundingBox();
		if (leopard->GetState() == ZOMBIE_INACTIVE)
			continue;

		leopard->Render();
		//	//zombie->RenderBoundingBox();
	}

	for (auto bat : listVampireBats)
	{
		if (bat->GetState() == VAMPIRE_BAT_INACTIVE)
			continue;

		bat->Render();
		bat->RenderBoundingBox();
	}
}

void SceneManager::SetDropItems(LPGAMEOBJECT object)
{
	//if (object->IsEnable() == false ||
	//	(dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_DESTROYED))
	//{
	//	if (object->idItem != -1 && object->IsDroppedItem() == false)
	//	{
	//		object->SetIsDroppedItem(true);

	//		// Tạo một item theo id

	//		item = new Items();
	//		item->SetEnable(true);
	//		item->SetPosition(object->x, object->y);
	//		item->SetItem(object->idItem);

	//		listItems.push_back(item);
	//		Objects.push_back(item);
	//	}
	//}
}

void SceneManager::SetInactivationByPosition()
{
	D3DXVECTOR2 entryViewPort = game->GetCamPos();

	for (auto zombie : listZombies)
	{
		Zombie* zb = dynamic_cast<Zombie*>(zombie);

		if (zb->GetState() == ZOMBIE_ACTIVE)
		{
			float zx, zy;
			zb->GetPosition(zx, zy);

			if (zx + ZOMBIE_BBOX_WIDTH < entryViewPort.x || zx > entryViewPort.x + SCREEN_WIDTH)
			{
				zb->SetState(ZOMBIE_INACTIVE);
			}
		}
	}

	for (auto leopard : listBlackLeopards)
	{
		BlackLeopard* bl = dynamic_cast<BlackLeopard*>(leopard);

		if (bl->GetState() == BLACK_LEOPARD_ACTIVE)
		{
			float lx, ly;
			bl->GetPosition(lx, ly);

			if (lx + BLACK_LEOPARD_BBOX_WIDTH < entryViewPort.x || lx > entryViewPort.x + SCREEN_WIDTH)
			{
				bl->SetState(BLACK_LEOPARD_INACTIVE);
			}
		}
	}

	for (auto bat : listVampireBats)
	{
		VampireBat* vb = dynamic_cast<VampireBat*>(bat);

		if (vb->GetState() == VAMPIRE_BAT_ACTIVE && vb->IsSettedPosition() == true)
		{
			float bx, by;
			vb->GetPosition(bx, by);

			if (bx + VAMPIRE_BAT_BBOX_WIDTH < entryViewPort.x || bx > entryViewPort.x + SCREEN_WIDTH)
			{
				vb->SetState(VAMPIRE_BAT_INACTIVE);
			}
		}
	}
}

void SceneManager::ChangeScene(int scene)
{
	IDScene = scene;

	switch (IDScene)
	{
	case SCENE_1:
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);
		CreateListChangeSceneObjects();
		simon->SetPosition(1200.0f, 302.0f);
		game->SetCamPos(0.0f, 0.0f);
		break;
	case SCENE_2:
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_2);
		CreateListChangeSceneObjects();
		simon->SetPosition(0.0f, 100.0f);
		game->SetCamPos(0.0f, 0.0f);
		break;
	case SCENE_3:
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_3);
		CreateListChangeSceneObjects();
		simon->SetPosition(93.0f, 48.0f);
		simon->SetState(STAIR_DOWN);
		game->SetCamPos(0.0f, 0.0f);
		break;
	default:
		break;
	}
}