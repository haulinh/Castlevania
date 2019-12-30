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

	boss = new Boss();

	tilemaps->Add(SCENE_1, FILEPATH_TEX_MAP_SCENE_1, FILEPATH_DATA_MAP_SCENE_1, 1536, 320, 32, 32);
	tilemaps->Add(SCENE_2, FILEPATH_TEX_MAP_SCENE_2, FILEPATH_DATA_MAP_SCENE_2, 5632, 352, 32, 32);
	tilemaps->Add(SCENE_3, FILEPATH_TEX_MAP_SCENE_3, FILEPATH_DATA_MAP_SCENE_3, 1024, 352, 32, 32);

	for (int i = 1; i <= 3; i++)
	{
		subweapon = new SubWeapon();
		subweapon->SetEnable(false);
		subweaponList.push_back(subweapon);
	}
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
			unit = new Unit(grid, candle, pos_x, pos_y);
		}
		else if (ID_Obj == GROUND)
		{
			ground = new Ground();
			ground->SetPosition(pos_x, pos_y);
			ground->SetState(state);
			ground->isEnable = isEnable;
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
			unit = new Unit(grid, leopard, pos_x, pos_y);
		}
		else if (ID_Obj == VAMPIRE_BAT)
		{
			bat = new VampireBat();
			bat->SetEntryPosition(pos_x, pos_y);
			bat->SetState(VAMPIRE_BAT_INACTIVE);
			bat->SetEnable(true);
			unit = new Unit(grid, bat, pos_x, pos_y);
		}
		else if (ID_Obj == FISHMAN)
		{
			fishman = new FishMan();
			fishman->SetEntryPosition(pos_x, pos_y);
			fishman->SetState(FISHMAN_INACTIVE);
			fishman->SetEnable(true);
			unit = new Unit(grid, fishman, pos_x, pos_y);
		}
		else if (ID_Obj == BOSS)
		{
			boss = new Boss();
			boss->SetEntryPosition(pos_x, pos_y);
			boss->SetPosition(pos_x, pos_y);
			boss->SetState(BOSS_INACTIVE);
			boss->SetIdItem(nameItem);
			boss->SetEnable(true);
			unit = new Unit(grid, boss, pos_x, pos_y);
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

	//DebugOut(L"%d list unit: \n", listUnits.size());

	for (int i = 0; i < listUnits.size(); i++)
	{
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (dynamic_cast<Candle*>(obj) || dynamic_cast<Door*>(obj))
			listStaticObjectsToRender.push_back(obj);

		else if (dynamic_cast<Zombie*>(obj) || dynamic_cast<BlackLeopard*>(obj) ||
			dynamic_cast<VampireBat*>(obj) || dynamic_cast<FishMan*>(obj) ||
			dynamic_cast<FireBall*>(obj) || dynamic_cast<Boss*>(obj) ||
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

					simon->SetState(STAIR_DOWN);
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

	// Stop Watch

	if (isUsingStopWatch == true && GetTickCount() - stopWatchCounter > WEAPONS_STOP_WATCH_TIME)
	{
		isUsingStopWatch = false;
		stopWatchCounter = 0;
	}

	// get object from grid by camera position
	GetObjectFromGrid();

	// Simon - Cross
	if (simon->IsCrossCollected() == true)
	{
		simon->SetCrossCollected(false);
		CrossEffect();
	}

	if (isCrossEffect == true && GetTickCount() - crossEffectTimeCounter >= ITEM_CROSS_EFFECT_TIME)
	{
		isCrossEffect = false;
		crossEffectTimeCounter = 0;
	}

	// double shot
	DoubleShotEffect();

	// triple shot
	TripleShotEffect();

	// Update
	Simon_Update(dt);

	for (int i = 0; i < 3; i++)
	{
		if (subweaponList[i]->IsEnable() == true)
		{
			Weapon_Update(dt, i);
		}
	}

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
		else if (dynamic_cast<Boss*>(object))
		{
			Boss_Update(dt, object);
		}
		else
		{
			object->Update(dt, &listObjects);
		}
	}

	if (isBossFighting == true && simon->x < game->GetCamPos().x)
	{
		simon->x = game->GetCamPos().x;
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
	tilemaps->Get(IDScene)->Draw(game->GetCamPos(), isCrossEffect);

	for (int i = 0; i < 3; i++)
	{
		if (subweaponList[i]->IsEnable() == true)
		{
			subweaponList[i]->Render();
			//subweaponList[i]->RenderBoundingBox();
		}
	}

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
		//obj->RenderBoundingBox();
	}

	simon->Render();
	//simon->RenderBoundingBox();
}

void SceneManager::UpdateCameraPosition()
{
	if (isBossFighting == true) // Boss fight -> not moving camera
		return;

	if (IDScene == SCENE_2 &&
		tilemaps->Get(IDScene)->index == 2 &&
		simon->x + SCREEN_WIDTH / 2 >= tilemaps->Get(IDScene)->GetMapWidth())
	{
		isBossFighting = true;
		return; // Boss fight
	}

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
		LPGAMEOBJECT obj = listUnits[i]->GetObj();

		if (obj->IsEnable() == false)
			continue;

		float newPos_x, newPos_y;
		obj->GetPosition(newPos_x, newPos_y);
		listUnits[i]->Move(newPos_x, newPos_y);
	}
}

void SceneManager::SetDropItems(LPGAMEOBJECT object)
{
	if ((dynamic_cast<Candle*>(object) && object->GetState() == CANDLE_DESTROYED) ||
		(dynamic_cast<Zombie*>(object) && object->GetState() == ZOMBIE_DESTROYED) ||
		(dynamic_cast<VampireBat*>(object) && object->GetState() == VAMPIRE_BAT_DESTROYED) ||
		(dynamic_cast<FishMan*>(object) && object->GetState() == FISHMAN_DESTROYED) ||
		(dynamic_cast<Boss*>(object) && object->GetState() == BOSS_DESTROYED))
	{
		if (object->nameItem != "" && object->IsDroppedItem() == false)
		{
			if (dynamic_cast<Boss*>(object))
			{
				boss = dynamic_cast<Boss*>(object);

				if (boss->DropItem() == true)
				{
					object->SetIsDroppedItem(true);
					boss->SetEnable(false);

					float ix = game->GetCamPos().x + SCREEN_WIDTH / 2;
					float iy = game->GetCamPos().y + SCREEN_HEIGHT / 2;

					item = new Items();
					item->SetEnable(true);
					item->SetPosition(ix, iy);
					item->SetItem(object->nameItem);

					listItems.push_back(item);
					unit = new Unit(grid, item, ix, iy);
				}
			}
			else
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

	for (int i = 0; i < 3; i++)
	{
		if (subweaponList[i]->IsEnable() == true)
		{
			float wx, wy;
			subweaponList[i]->GetPosition(wx, wy);

			if (wx < entryViewPort.x || wx > entryViewPort.x + SCREEN_WIDTH ||
				wy < entryViewPort.y || wy > entryViewPort.y + SCREEN_HEIGHT)
			{
				subweaponList[i]->SetEnable(false);
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
		//simon->SetPosition(5344.0f, 335.0f);
		//game->SetCamPos(4080.0f, 0.0f);
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

void SceneManager::ResetGameState()
{
	isSimonDead = false;
	simonDeadTimeCounter = 0;

	simon->SetState(IDLE);
	simon->SetHP(SIMON_HP);
	simon->SetEnergy(SIMON_ENERGY);

	simon->GetWeapon()->SetState(MAGIC_WHIP);

	simon->SetSubWeapon("");

	switch (IDScene)
	{
	case SCENE_1:
		ChangeScene(SCENE_1);
		break;
	case SCENE_2:
	case SCENE_3:
		ChangeScene(SCENE_2);
		if (IDScene == SCENE_3 ||
			(IDScene == SCENE_2 && tilemaps->Get(IDScene)->index == 1))
		{
			simon->SetPosition(3115.0f, 143.0f);
			game->SetCamPos(3056.0f, 0.0f);
		}
		else if (IDScene == SCENE_2 && tilemaps->Get(IDScene)->index == 2)
		{
			isBossFighting = false;
			simon->SetPosition(4139.0f, 143.0f);
			game->SetCamPos(4079.0f, 0.0f);
		}
		break;
	default:
		break;
	}

}

void SceneManager::CrossEffect()
{
	isCrossEffect = true;
	crossEffectTimeCounter = GetTickCount();

	for (UINT i = 0; i < listObjects.size(); i++)
	{
		if (dynamic_cast<Candle*>(listObjects[i]))
		{
			candle = dynamic_cast<Candle*>(listObjects[i]);
			candle->SetState(CANDLE_DESTROYED);
		}
		else if (dynamic_cast<Zombie*>(listObjects[i]))
		{
			zombie = dynamic_cast<Zombie*>(listObjects[i]);
			zombie->SetState(ZOMBIE_DESTROYED);
		}
		else if (dynamic_cast<BlackLeopard*>(listObjects[i]))
		{
			leopard = dynamic_cast<BlackLeopard*>(listObjects[i]);
			leopard->SetState(BLACK_LEOPARD_DESTROYED);
		}
		else if (dynamic_cast<VampireBat*>(listObjects[i]))
		{
			bat = dynamic_cast<VampireBat*>(listObjects[i]);
			bat->SetState(VAMPIRE_BAT_DESTROYED);
		}
		else if (dynamic_cast<FishMan*>(listObjects[i]))
		{
			fishman = dynamic_cast<FishMan*>(listObjects[i]);
			fishman->SetState(FISHMAN_DESTROYED);
		}
		else if (dynamic_cast<Boss*>(listObjects[i]))
		{
			boss = dynamic_cast<Boss*>(listObjects[i]);
			boss->SetState(BOSS_DESTROYED);
		}
	}
}

void SceneManager::DoubleShotEffect()
{
	if (simon->IsGotDoubleShotItem())
	{
		simon->SetGotDoubleShotItem(false);
		isDoubleShotEffect = true;
		doubleShotEffectTimeCounter = GetTickCount();
	}
	else if (isDoubleShotEffect == true && GetTickCount() - doubleShotEffectTimeCounter > ITEM_DOUBLE_SHOT_EFFECT_TIME)
	{
		isDoubleShotEffect = false;
		doubleShotEffectTimeCounter = 0;
	}
}

void SceneManager::TripleShotEffect()
{
	if (simon->IsGotTripleShotItem())
	{
		simon->SetGotTripleShotItem(false);
		isTripleShotEffect = true;
		tripleShotEffectTimeCounter = GetTickCount();
	}
	else if (isTripleShotEffect == true && GetTickCount() - tripleShotEffectTimeCounter > ITEM_TRIPLE_SHOT_EFFECT_TIME)
	{
		isTripleShotEffect = false;
		tripleShotEffectTimeCounter = 0;
	}
}

void SceneManager::Simon_Update(DWORD dt)
{
	if (simon->GetState() == DEAD)
	{
		if (isSimonDead == false)
		{
			StartSimonDeadTimeCounter();
		}
		else if (GetTickCount() - simonDeadTimeCounter >= SIMON_DEAD_TIME)
		{
			ResetGameState();
		}

		return;
	}

	vector<LPGAMEOBJECT> coObjects;

	for (auto obj : listObjects)
	{
		if ((dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj) || dynamic_cast<Door*>(obj) ||
			dynamic_cast<FireBall*>(obj) || dynamic_cast<Zombie*>(obj) ||
			dynamic_cast<BlackLeopard*>(obj) || dynamic_cast<VampireBat*>(obj) ||
			dynamic_cast<FishMan*>(obj) || dynamic_cast<Boss*>(obj)) &&
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

void SceneManager::Weapon_Update(DWORD dt, int index)
{
	if (subweaponList[index]->GetState() == STOP_WATCH_SUB)
		return;

	if (subweaponList[index]->IsEnable() == false)
		return;

	vector<LPGAMEOBJECT> coObjects;
	coObjects.push_back(simon); // dùng để xét va chạm của Simon với boomerang

	for (auto obj : listObjects)
	{
		if ((dynamic_cast<Candle*>(obj) || dynamic_cast<Ground*>(obj) ||
			dynamic_cast<FireBall*>(obj) || dynamic_cast<Zombie*>(obj) ||
			dynamic_cast<BlackLeopard*>(obj) || dynamic_cast<VampireBat*>(obj) ||
			dynamic_cast<FishMan*>(obj) || dynamic_cast<Boss*>(obj)) &&
			obj->GetState() != INACTIVE && obj->IsEnable() == true)
		{
			coObjects.push_back(obj);
		}
	}

	subweaponList[index]->Update(dt, &coObjects);
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

		object->SetStopMovement(isUsingStopWatch);
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

		object->SetStopMovement(isUsingStopWatch);
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

		object->SetStopMovement(isUsingStopWatch);
		bat->Update(dt, NULL);
	}
}

void SceneManager::FishMan_Update(DWORD dt, LPGAMEOBJECT& object)
{
	fishman = dynamic_cast<FishMan*>(object);

	if (fishman->GetState() != FISHMAN_INACTIVE)
	{
		if (fishman->GetState() == FISHMAN_ACTIVE && fishman->IsSettedPosition() == true &&
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
			if (fishman->IsSettedPosition() == false)
			{
				fishman->SetIsSettedPosition(true);

				float simon_x, simon_y;
				simon->GetPosition(simon_x, simon_y);

				int nx = simon_x > fishman->GetEntryPosition().x ? 1 : -1;
				fishman->SetN(nx);

				float distance = 50 + rand() % 50;

				fishman->SetPosition(simon_x - nx * distance, fishman->GetEntryPosition().y);

				fishman->SetState(FISHMAN_JUMP);
			}

			vector<LPGAMEOBJECT> coObjects;

			for (auto obj : listObjects)
			{
				if (dynamic_cast<Ground*>(obj))
				{
					coObjects.push_back(obj);
				}
			}

			object->SetStopMovement(isUsingStopWatch);
			fishman->Update(dt, &coObjects);
		}
	}
	else if (fishman->IsRenderingBubbles() == true)
	{
		fishman->Update(dt);
	}
}

void SceneManager::Boss_Update(DWORD dt, LPGAMEOBJECT& object)
{
	if (object->GetState() == BOSS_INACTIVE)
		return;

	boss = dynamic_cast<Boss*>(object);

	// Passing simon's position for boss movement
	float sx, sy;
	simon->GetPosition(sx, sy);
	boss->SetSimonPosition(sx, sy);

	boss->Update(dt);
}

