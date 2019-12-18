#include "SceneManager.h"
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
	//LoadResourceFile->LoadAllResource();
	LoadResourceFile->LoadData("resources\\data.xml");


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
			unit = new Unit(grid, candle, pos_x, pos_y);
		}
		else if (ID_Obj == GROUND)
		{
			ground = new Ground();
			ground->SetPosition(pos_x, pos_y);
			ground->SetState(state);
			ground->isEnable = isEnable;
			listGrounds.push_back(ground);
			unit = new Unit(grid, ground, pos_x, pos_y);
		}
		else if (ID_Obj == STAIR)
		{
			stair = new Stair();
			stair->SetPosition(pos_x, pos_y);
			stair->SetState(state);
			stair->SetEnable(isEnable);
			stair->SetType(typeStair);
			listStairs.push_back(stair);
			unit = new Unit(grid, stair, pos_x, pos_y);
		}
		else if (ID_Obj == DOOR)
		{
			door = new Door();
			door->SetPosition(pos_x, pos_y);
			door->SetState(state);
			door->SetEnable(isEnable);
			listDoors.push_back(door);
			unit = new Unit(grid, door, pos_x, pos_y);
		}
		else if (ID_Obj == ZOMBIE)
		{
			zombie = new Zombie();
			zombie->SetEntryPosition(pos_x, pos_y);
			zombie->SetState(state);
			zombie->SetEnable(isEnable);
			unit = new Unit(grid, zombie, pos_x, pos_y);
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
			unit = new Unit(grid, leopard, pos_x, pos_y);
		}
		else if (ID_Obj == VAMPIRE_BAT)
		{
			bat = new VampireBat();
			bat->SetEntryPosition(pos_x, pos_y);
			bat->SetState(VAMPIRE_BAT_INACTIVE);
			bat->SetEnable(true);
			listVampireBats.push_back(bat);
			unit = new Unit(grid, bat, pos_x, pos_y);
		}
		else if (ID_Obj == FISHMAN)
		{
			fishman = new FishMan();
			fishman->SetEntryPosition(pos_x, pos_y);
			fishman->SetState(FISHMAN_INACTIVE);
			fishman->SetEnable(true);
			listFishMans.push_back(fishman);
			unit = new Unit(grid, fishman, pos_x, pos_y);
		}

	}
	fs.close();

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

void SceneManager::GetObjectFromGrid()
{
	listUnits.clear();
	listObjects.clear();
	listStaticObjectsToRender.clear();
	listMovingObjectsToRender.clear();

	grid->Get(game->GetCamPos(), listUnits);

	DebugOut(L"%d list unit: \n", listUnits.size());

	for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (dynamic_cast<Candle*>(obj) || dynamic_cast<Door*>(obj))
			listStaticObjectsToRender.push_back(obj);

		else if (dynamic_cast<Zombie*>(obj) || dynamic_cast<BlackLeopard*>(obj) ||
			dynamic_cast<VampireBat*>(obj) || dynamic_cast<FishMan*>(obj) ||
			dynamic_cast<FireBall*>(obj) ||
			dynamic_cast<Items*>(obj))
			listMovingObjectsToRender.push_back(obj);

		listObjects.push_back(obj);

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
					if (x < 3200.0f) simon->SetPosition(100.0f, 48.0f);
					else simon->SetPosition(740.0f, 48.0f);

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


	// get object from grid by camera position
	GetObjectFromGrid();

	// Update
	Simon_Update(dt);
	Weapon_Update(dt);

	for (UINT i = 0; i < listObjects.size(); i++)
	{
		LPGAMEOBJECT object = listObjects[i];

		SetDropItems(object);

		if (dynamic_cast<Items*>(object))
		{
			Item_Update(dt, object);
		}
		else if (dynamic_cast<Zombie*>(object))
		{
			Zombie_Update(dt, object);
		}
		else if (dynamic_cast<BlackLeopard*>(object))
		{
			BlackLeopard_Update(dt, object);
		}
		else if (dynamic_cast<VampireBat*>(object))
		{
			VampireBat_Update(dt, object);
		}
		else if (dynamic_cast<FishMan*>(object))
		{
			FishMan_Update(dt, object);
		}
		else
		{
			object->Update(dt, &listObjects);
		}
	}

	//
	SetInactivationByPosition();

	// update camera
	UpdateCameraPosition();

	// update grid
	UpdateGrid();
}

void SceneManager::Render()
{
	tilemaps->Get(IDScene)->Draw(game->GetCamPos());

	for (auto obj : listStaticObjectsToRender)
	{
		obj->Render();
		//obj->RenderBoundingBox();
	}

	for (auto obj : listMovingObjectsToRender)
	{
		obj->Render();
		//obj->RenderBoundingBox();
		//obj->RenderActiveBoundingBox();
	}

	for (auto obj : listStairs)
	{
		obj->RenderBoundingBox();
		//obj->RenderActiveBoundingBox();
	}

	for (auto obj : listGrounds)
	{
		obj->RenderBoundingBox();
		//obj->RenderActiveBoundingBox();
	}

	simon->Render();
	//simon->RenderBoundingBox();

	if (subweapon->IsEnable() == true)
	{
		subweapon->Render();
		subweapon->RenderBoundingBox();
	}
}

void SceneManager::UpdateCameraPosition()
{

	if (simon->x > SCREEN_WIDTH / 2 &&
		simon->x + SCREEN_WIDTH / 2 < tilemaps->Get(IDScene)->GetMapWidth())
	{
		TileMap* map = tilemaps->Get(IDScene);
		int min_col = map->min_max_col_to_draw[map->index][0];
		int max_col = map->min_max_col_to_draw[map->index][1];

		if (simon->x >= min_col * 32 + (SCREEN_WIDTH / 2 - 16) &&
			simon->x <= max_col * 32 - (SCREEN_WIDTH / 2 - 16))
		{
			game->SetCamPos(simon->x - SCREEN_WIDTH / 2, 0);
		}
	}
}

void SceneManager::UpdateGrid()
{
	for (int i = 0; i < listUnits.size(); i++)
	{
		float newPos_x, newPos_y;
		listUnits[i]->GetObj()->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
	}
}

void SceneManager::SetDropItems(LPGAMEOBJECT object)
{
	if ((dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROY) ||
		(dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_DESTROYED) ||
		(dynamic_cast<VampireBat*>(object) && object->GetState() == VAMPIRE_BAT_DESTROYED) ||
		(dynamic_cast<FishMan*>(object) && object->GetState() == FISHMAN_DESTROYED))
	{
		if (object->nameItem != "" && object->IsDroppedItem() == false)
		{
			object->SetIsDroppedItem(true);

			// Tạo một item theo id

			item = new Items();
			item->SetEnable(true);
			item->SetPosition(object->x, object->y);
			item->SetItem(object->nameItem);

			listItems.push_back(item);
			unit = new Unit(grid, item, object->x, object->y);
		}
	}
}

void SceneManager::SetInactivationByPosition()
{
	D3DXVECTOR2 entryViewPort = game->GetCamPos();

	for (auto object : listObjects)
	{
		float x, y;
		object->GetPosition(x, y);

		if (x < entryViewPort.x || x > entryViewPort.x + SCREEN_WIDTH - 1)
		{
			if (dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_ACTIVE)
			{
				zombie = dynamic_cast<Zombie*>(object);
				zombie->SetState(ZOMBIE_INACTIVE);
			}
			else if (dynamic_cast<BlackLeopard*>(object) && object->GetState() == BLACK_LEOPARD_ACTIVE)
			{
				leopard = dynamic_cast<BlackLeopard*>(object);
				leopard->SetState(BLACK_LEOPARD_INACTIVE);
			}
			else if (dynamic_cast<VampireBat*>(object) && object->GetState() == VAMPIRE_BAT_ACTIVE)
			{
				bat = dynamic_cast<VampireBat*>(object);

				if (bat->IsSettedPosition())
				{
					bat->SetState(VAMPIRE_BAT_INACTIVE);
				}
			}
			else if (dynamic_cast<FishMan*>(object))
			{
				fishman = dynamic_cast<FishMan*>(object);
				fishman->SetState(FISHMAN_INACTIVE);
			}
			else if (dynamic_cast<FireBall*>(object))
			{
				object->SetEnable(false);
			}
		}
		else if (dynamic_cast<FishMan*>(object) && object->GetState() != FISHMAN_INACTIVE)
		{
			fishman = dynamic_cast<FishMan*>(object);

			if (x < entryViewPort.x || x > entryViewPort.x + SCREEN_WIDTH - 1 ||
				y < entryViewPort.y || y > fishman->GetEntryPosition().y)
			{
				if (y >= fishman->GetEntryPosition().y) // rơi xuống nước mới có bọt nước
					fishman->SetIsNeedToCreateBubbles(true);

				fishman->SetState(FISHMAN_INACTIVE);
			}
		}
	}

	if (subweapon->IsEnable() == true)
	{
		float wx, wy;
		subweapon->GetPosition(wx, wy);

		if (wx < entryViewPort.x || wx > entryViewPort.x + SCREEN_WIDTH ||
			wy < entryViewPort.y || wy > entryViewPort.y + SCREEN_HEIGHT)
		{
			subweapon->SetEnable(false);
		}
	}
}

void SceneManager::ChangeScene(int scene)
{
	IDScene = scene;

	switch (IDScene)
	{
	case SCENE_1:
		grid = new Grid(1536, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);
		CreateListChangeSceneObjects();
		simon->SetPosition(0.0f, 302.0f);
		game->SetCamPos(0.0f, 0.0f);
		break;
	case SCENE_2:
		grid = new Grid(5632, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_2);
		CreateListChangeSceneObjects();
		//simon->SetPosition(2560.0f, 335.0f);
		//game->SetCamPos(0.0f, 0.0f);
		simon->SetPosition(0.0f, 335.0f);
		game->SetCamPos(0.0f, 0.0f);
		break;
	case SCENE_3:
		grid = new Grid(1024, 480, DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT);
		LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_3);
		CreateListChangeSceneObjects();
		simon->SetPosition(100.0f, 48.0f);
		simon->SetState(STAIR_DOWN);
		game->SetCamPos(0.0f, 0.0f);
		break;
	default:
		break;
	}
}

void SceneManager::Simon_Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;

	for (auto obj : listObjects)
	{
		if ((dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<Door*>(obj) ||
			dynamic_cast<FireBall*>(obj) || dynamic_cast<Zombie*>(obj) ||
			dynamic_cast<BlackLeopard*>(obj) || dynamic_cast<VampireBat*>(obj) ||
			dynamic_cast<FishMan*>(obj)) &&
			obj->GetState() != INACTIVE && obj->IsEnable() == true)
		{
			coObjects.push_back(obj);
		}
	}

	simon->Update(dt, &coObjects);
	simon->CheckCollisionWithItem(&listItems);
	simon->CheckChangeScene(&listChangeSceneObjs);
	simon->CheckCollisionWithEnemyActiveArea(&listObjects);
}

void SceneManager::Weapon_Update(DWORD dt)
{

	if (subweapon->IsEnable() == false)
		return;

	vector<LPGAMEOBJECT> coObjects;
	coObjects.push_back(simon); // dùng để xét va chạm của Simon với boomerang

	for (auto obj : listObjects)
	{
		if ((dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj) ||
			dynamic_cast<FireBall*>(obj) || dynamic_cast<Zombie*>(obj) ||
			dynamic_cast<BlackLeopard*>(obj) || dynamic_cast<VampireBat*>(obj) ||
			dynamic_cast<FishMan*>(obj)) &&
			obj->GetState() != INACTIVE && obj->IsEnable() == true)
		{
			coObjects.push_back(obj);
		}
	}

	subweapon->Update(dt, &coObjects);
}

void SceneManager::Item_Update(DWORD dt, LPGAMEOBJECT& item)
{
	if (item->IsEnable() == true)
	{
		vector<LPGAMEOBJECT> coObjects;

		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
			{
				coObjects.push_back(obj);
			}
		}

		item->Update(dt, &coObjects);
	}
}

void SceneManager::Zombie_Update(DWORD dt, LPGAMEOBJECT& object)
{
	if (object->GetState() != ZOMBIE_INACTIVE)
	{
		zombie = dynamic_cast<Zombie*>(object);

		if (zombie->IsSettedPosition() == false)
		{
			zombie->SetIsSettedPosition(true);

			D3DXVECTOR2 entryPos = zombie->GetEntryPosition();
			if (zombie->GetN() == -1) zombie->SetPosition(game->GetCamPos().x + SCREEN_WIDTH - ENEMY_DEFAULT_BBOX_WIDTH, entryPos.y);
			else zombie->SetPosition(game->GetCamPos().x + ENEMY_DEFAULT_BBOX_WIDTH, entryPos.y);
		}

		vector<LPGAMEOBJECT> coObjects;

		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
			{
				coObjects.push_back(obj);
			}
		}

		object->Update(dt, &coObjects);
	}

}

void SceneManager::BlackLeopard_Update(DWORD dt, LPGAMEOBJECT& object)
{
	if (object->GetState() != BLACK_LEOPARD_INACTIVE)
	{
		vector<LPGAMEOBJECT> coObjects;

		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
			{
				coObjects.push_back(obj);
			}
		}

		object->Update(dt, &coObjects);
	}
}

void SceneManager::VampireBat_Update(DWORD dt, LPGAMEOBJECT& object)
{
	if (object->GetState() != VAMPIRE_BAT_INACTIVE)
	{
		bat = dynamic_cast<VampireBat*>(object);

		if (bat->IsSettedPosition() == false)
		{
			bat->SetIsSettedPosition(true);

			// set random hướng cho dơi

			int listNx[2] = { -1, 1 };
			int rndIndex = rand() % 2;
			bat->SetN(listNx[rndIndex]);

			// set vị trí cho dơi luôn 
			// dơi bay ngang tầm simon, từ phía cuối của 2 đầu màn hình)
			float bx, by;

			by = simon->y + 20;

			if (bat->GetN() == -1) bx = game->GetCamPos().x + SCREEN_WIDTH - ENEMY_DEFAULT_BBOX_WIDTH;
			else bx = game->GetCamPos().x + ENEMY_DEFAULT_BBOX_WIDTH;

			bat->SetPosition(bx, by);
			bat->SetState(VAMPIRE_BAT_ACTIVE);
		}

		bat->Update(dt, NULL);
	}
}

void SceneManager::FishMan_Update(DWORD dt, LPGAMEOBJECT& object)
{
	fishman = dynamic_cast<FishMan*>(object);

	if (fishman->GetState() == FISHMAN_ACTIVE &&
		GetTickCount() - fishman->GetLastTimeShoot() >= fishman->GetDeltaTimeToShoot())
	{
		fishman->SetState(FISHMAN_HIT);

		// Tạo fireball
		float fx, fy, nx;
		fishman->GetPosition(fx, fy);
		nx = fishman->GetN();

		fireball = new FireBall();
		fireball->SetPosition(fx + 5.0f, fy + 10.0f);
		fireball->SetN(nx);
		fireball->SetState(FIREBALL);
		fireball->SetEnable(true);

		unit = new Unit(grid, fireball, fx + 5.0f, fy + 10.0f);

		// Đặt hướng quay mặt của Fishman sau khi bắn (quay về phía simon)
		float sx, sy;
		simon->GetPosition(sx, sy);

		if (fx < sx) fishman->SetNxAfterShoot(1);
		else fishman->SetNxAfterShoot(-1);
	}
	else
	{
		if (fishman->GetState() == FISHMAN_JUMP && fishman->IsSettedPosition() == false)
		{
			float simon_x, simon_y;
			simon->GetPosition(simon_x, simon_y);

			if (abs(simon_x - fishman->GetEntryPosition().x) < 50.0f)
				return;

			int nx = simon_x > fishman->GetEntryPosition().x ? 1 : -1;
			fishman->SetN(nx);

			fishman->SetState(FISHMAN_JUMP);
			fishman->SetIsSettedPosition(true);
		}

		vector<LPGAMEOBJECT> coObjects;

		for (auto obj : listObjects)
		{
			if (dynamic_cast<Ground*>(obj))
			{
				coObjects.push_back(obj);
			}
		}

		fishman->Update(dt, &coObjects);
	}
}

