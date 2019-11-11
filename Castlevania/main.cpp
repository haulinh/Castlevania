/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		Game::SweptAABB
		GameObject::SweptAABBEx
		GameObject::CalcPotentialCollisions
		GameObject::FilterCollision

		GameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "define.h"

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "simon.h"
#include "Brick.h"
#include "Whip.h"
#include <iostream>
#include "TileMap.h"
#include "Candle.h"

Game *game;

Simon *simon;
Whip* whip;
TileMap* tilemap;

vector<LPGAMEOBJECT> objects;

class CSampleKeyHander: public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler; 

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	if (simon->IsJumping()) return;
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(Jump);
		break;
	case DIK_D:
		simon->SetState(Attack);
		break;
	}
	
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE* states)
{
	if (simon->IsJumping()) return;
	
	if (simon->IsAttacking()) return;

	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(Sit);

	else if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetN(1);
		simon->SetState(Walking);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetN(-1);
		simon->SetState(Walking);
	}

	else 
		simon->SetState(Idle);

}
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

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures* textures = CTextures::GetInstance();
	textures->Add(400, L"resources\\Map\\map1.png", D3DCOLOR_XRGB(255, 0, 255));
	auto texMap = textures->Get(400);
	tilemap = new TileMap(L"resources\\Map\\map1.txt", 1536, 320, 32, 32);
	tilemap->SetTileMap(texMap);
	tilemap->LoadResources();


	CAnimations * animations = CAnimations::GetInstance();
	LoadResourceFile* LoadResourceFile = LoadResourceFile::GetInstance();

	LoadResourceFile->LoadAllResource();

	LPANIMATION ani;

	//for (int i = 0; i < 5; i++)
	//{
	//	Candle* candle = new Candle();
	//	candle->SetId(i);
	//	candle->SetPosition(160 + i * 270, 320 - 64 - 32);
	//	objects.push_back(candle);
	//}

	simon = new Simon();
	simon->SetPosition(200.0f, 300-60-32);
	objects.push_back(simon);

	//whip = new Whip();
	//whip->SetPosition(simon->x, simon->y);

	for (int i = 0; i < 100; i++)
	{
		Brick* brick = new Brick();
		brick->AddAnimation("brick");
		brick->SetPosition(0 + i * 16.0f, 320 - 32);
		objects.push_back(brick);
	}

}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{

		objects[i]->Update(dt,&coObjects);
	/*	if (objects[i]->GetState() == isEnable)
		{
			objects.erase(objects.begin() + i);
		}*/
	}

	/* Update camera to follow simon*/
	if (simon->x >= SCREEN_WIDTH / 2 && simon->x <= TILEMAP1_WIDTH - SCREEN_WIDTH/2)
	{
		game->SetCamPos(simon->x - 320, 0);
	}
	
	float cx, cy;
	cx = simon->x;
	cy = simon->y;
	if (cx > SCREEN_WIDTH / 2 && cx + SCREEN_WIDTH / 2 < tilemap->GetMapWidth())
	{
		game->SetCamPos(cx - SCREEN_WIDTH / 2, 0);
	}
}

/*
	Render a frame 
*/
void Render()
{
	CTextures* textures = CTextures::GetInstance();
	auto d3ddv = game->GetDirect3DDevice();
	auto bb = game->GetBackBuffer();
	auto spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		tilemap->Draw(game->GetCamPos());


		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}