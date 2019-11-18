#include <windows.h>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>

#include "define.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "KeyBoardInput.h"

#include "Simon.h"
#include "Brick.h"
#include "Weapon.h"
#include "TileMap.h"
#include "Candle.h"
#include "Items.h"
#include "Dagger.h"
#include "ScenceManager.h"

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight);
void Render();
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//Game* game;
//TileMap* tilemap;
//KeyBoardInput* input;
//
//Simon* simon;
//Dagger* dagger;

KeyBoardInput* input;
Game* game;
SceneManager* scenes;

vector<LPGAMEOBJECT> objects;

Textures* textures = Textures::GetInstance();
Animations* animations = Animations::GetInstance();

//void LoadResources()
//{
//	LoadResourceFile* LoadResourceFile = LoadResourceFile::GetInstance();
//	LoadResourceFile->LoadAllResource();
//
//	LPANIMATION ani;
//
//	dagger = new Dagger();
//	dagger->isEnable = false;
//	objects.push_back(dagger);
//
//	vector<int> listItem = { 0, 1, 0, 1, 2 };
//	for (int i = 0; i < listItem.size(); i++)
//	{
//		Candle* candle = new Candle();
//		candle->SetPosition(160 + i * 270, 320 - 64 - 32);
//		candle->SetIdItem(listItem[i]);
//		objects.push_back(candle);
//	}
//
//	for (int i = 0; i < 100; i++)
//	{
//		Brick* brick = new Brick();
//		brick->AddAnimation("brick");
//		brick->SetPosition(0 + i * 16.0f, 320 - 32);
//		objects.push_back(brick);
//	}
//
//	simon = new Simon();
//	simon->SetPosition(0.0f, 300 - 60 - 32);
//	objects.push_back(simon);
//
//}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	//for (int i = 0; i < objects.size(); i++)
	//{

	//	if (objects[i]->isEnable == false)
	//		continue;

	//	vector<LPGAMEOBJECT> coObjects;

	//	if (dynamic_cast<Simon*>(objects[i]))
	//	{
	//		for (int j = 0; j < objects.size(); j++)
	//		{
	//			if (objects[j]->isEnable == false)
	//				continue;

	//			if (i != j) // thêm tất cả objects "ko phải là simon", dùng trong hàm update của simon 
	//				coObjects.push_back(objects[j]);
	//		}
	//	}

	//	else if (dynamic_cast<Items*>(objects[i]))
	//	{
	//		for (int j = 0; j < objects.size(); j++)
	//		{
	//			if (objects[i]->isEnable == false)
	//				continue;

	//			if (dynamic_cast<Brick*>(objects[j])) // thêm tất cả objects "là ground", dùng trong hàm update của item
	//			{
	//				coObjects.push_back(objects[j]);
	//			}
	//		}
	//	}
	//	else if (dynamic_cast<Dagger*>(objects[i]))
	//	{
	//		for (int j = 0; j < objects.size(); j++)
	//		{
	//			if (objects[j]->isEnable == false)
	//				continue;

	//			if (i != j) // thêm tất cả objects "ko phải là dagger", dùng trong hàm update của dagger
	//				coObjects.push_back(objects[j]);
	//		}
	//	}
	//	else
	//	{
	//		coObjects.push_back(objects[i]);
	//	}

	//	if (dynamic_cast<Candle*>(objects[i])) // ham update Candle co them para &object de them item vao object
	//	{
	//		Candle* e = dynamic_cast<Candle*>(objects[i]);
	//		e->Update(dt, &objects, &coObjects);
	//	}
	//	else objects[i]->Update(dt, &coObjects);
	//}


	//// render camera
	//float cx, cy;
	//simon->GetPosition(cx, cy);

	//if (cx > SCREEN_WIDTH / 2 && cx + SCREEN_WIDTH / 2 < tilemap->GetMapWidth())
	//	game->SetCamPos(cx - SCREEN_WIDTH / 2, 0);

	scenes->Update(dt);
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

void Render()
{
	Textures* textures = Textures::GetInstance();
	auto d3ddv = game->GetDirect3DDevice();
	auto bb = game->GetBackBuffer();
	auto spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		//tilemap->Draw(game->GetCamPos());

		scenes->Render();

	/*	for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->isEnable == false)
				continue;
			objects[i]->Render();
		}*/

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);

	//LoadResources();

	scenes = new SceneManager(game, SCENE_1);
	scenes->LoadResources();
	scenes->LoadObjectsFromFile(FILEPATH_OBJECTS_SCENE_1);

	input = new KeyBoardInput(game, scenes->GetSimon());
	game->InitKeyboard(input);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}

#pragma region CreateGameWindow
HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}
#pragma endregion CreateGameWindow

#pragma region WinProc
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
#pragma endregion WinProc