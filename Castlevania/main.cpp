/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions: 
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "simon.h"
#include "Brick.h"
#include "Goomba.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(32, 32, 32)
#define SCREEN_WIDTH 640 
#define SCREEN_HEIGHT 480 

#define MAX_FRAME_RATE 60

CGame *game;

CMario *mario;
CGoomba *goomba;
CSimon *simon;

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
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetLevel(SIMON_LEVEL_BIG);
		simon->SetPosition(50.0f,0.0f);
		simon->SetSpeed(0, 0);
		break;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	else
		simon->SetState(SIMON_STATE_IDLE);
}

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

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add("id_tex_simon", L"textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add("id_tex_brick", L"textures\\brick.png", D3DCOLOR_XRGB(255, 0, 255));

	//textures->Add("ID_TEX_MARIO", L"textures\\mario.png",D3DCOLOR_XRGB(255, 255, 255));
	//textures->Add("ID_TEX_MISC", L"textures\\misc.png", D3DCOLOR_XRGB(176, 224, 248));
	//textures->Add("ID_TEX_ENEMY", L"textures\\enemies.png", D3DCOLOR_XRGB(3, 26, 110));

	//textures->Add("ID_TEX_BBOX", L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	//sprite simon
	auto texBrick = textures->Get("id_tex_brick");
	sprites->Add("brick", 0, 0, 32, 32, texBrick); 

	auto texSimon = textures->Get("id_tex_simon");
	sprites->Add("simon_idle_left", 0, 0, 60, 66, texSimon); // left 1
	sprites->Add("simon_left1", 60, 0, 60*2, 66, texSimon); // left 2
	sprites->Add("simon_left2", 60*2, 0, 60*3, 66, texSimon); // left 3
	sprites->Add("simon_left3", 60*3, 0, 60*4, 66, texSimon); // left 4

#pragma region spriteMario 
	//auto texMario = textures->Get("ID_TEX_MARIO");
	//// big
	//sprites->Add(10001, 246, 154, 260, 181, texMario);		// idle right 
	//sprites->Add(10002, 275, 154, 290, 181, texMario);		// walk
	//sprites->Add(10003, 304, 154, 321, 181, texMario);

	//sprites->Add(10011, 186, 154, 200, 181, texMario);		// idle left
	//sprites->Add(10012, 155, 154, 170, 181, texMario);		// walk
	//sprites->Add(10013, 125, 154, 140, 181, texMario);

	//sprites->Add(10099, 215, 120, 231, 135, texMario);		// die 

	//// small
	//sprites->Add(10021, 247, 0, 259, 15, texMario);			// idle small right
	//sprites->Add(10022, 275, 0, 291, 15, texMario);			// walk 
	//sprites->Add(10023, 306, 0, 320, 15, texMario);			// 

	//sprites->Add(10031, 187, 0, 198, 15, texMario);			// idle small left

	//sprites->Add(10032, 155, 0, 170, 15, texMario);			// walk
	//sprites->Add(10033, 125, 0, 139, 15, texMario);			// 


	//auto texMisc = textures->Get("ID_TEX_MISC");
	//sprites->Add(20001, 408, 225, 424, 241, texMisc);

	//auto texEnemy = textures->Get("ID_TEX_ENEMY");
	//sprites->Add(30001, 5, 14, 21, 29, texEnemy);
	//sprites->Add(30002, 25, 14, 41, 29, texEnemy); sprites->Add(30003, 45, 21, 61, 29, texEnemy); // die sprite
#pragma endregion SPRITEmario


	LPANIMATION ani;

	//brick
	ani = new CAnimation(100);
	ani->Add("brick");
	animations->Add("brick", ani);

	//simon walk
	ani = new CAnimation(100);
	ani->Add("simon_idle_left");
	ani->Add("simon_left1");
	ani->Add("simon_left2");
	ani->Add("simon_left3");
	animations->Add("simon_left", ani);

	//simon idle
	ani = new CAnimation(100);
	ani->Add("simon_idle_left");
	animations->Add("simon_idle_left", ani);

#pragma region animationMario
	//ani = new CAnimation(100);	// idle big right
	//ani->Add(10001);
	//animations->Add(400, ani);

	//ani = new CAnimation(100);	// idle big left
	//ani->Add(10011);
	//animations->Add(401, ani);

	//ani = new CAnimation(100);	// idle small right
	//ani->Add(10021);
	//animations->Add(402, ani);

	//ani = new CAnimation(100);	// idle small left
	//ani->Add(10031);
	//animations->Add(403, ani);

	//ani = new CAnimation(100);	// walk right big
	//ani->Add(10001);
	//ani->Add(10002);
	//ani->Add(10003);
	//animations->Add(500, ani);

	//ani = new CAnimation(100);	// // walk left big
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//animations->Add(501, ani);

	//ani = new CAnimation(100);	// walk right small
	//ani->Add(10021);
	//ani->Add(10022);
	//ani->Add(10023);
	//animations->Add(502, ani);

	//ani = new CAnimation(100);	// walk left small
	//ani->Add(10031);
	//ani->Add(10032);
	//ani->Add(10033);
	//animations->Add(503, ani);


	//ani = new CAnimation(100);		// Mario die
	//ani->Add(10099);
	//animations->Add(599, ani);



	//ani = new CAnimation(100);		// brick
	//ani->Add(20001);
	//animations->Add(601, ani);

	//ani = new CAnimation(300);		// Goomba walk
	//ani->Add(30001);
	//ani->Add(30002);
	//animations->Add(701, ani);

	//ani = new CAnimation(1000);		// Goomba dead
	//ani->Add(30003);
	//animations->Add(702, ani);
#pragma endregion


	//CBrick*simon = new CBrick();
	//simon->AddAnimation(300);
	//simon->SetPosition(0, 200);
	//objects.push_back(simon);

	//mario = new CMario();
	//mario->AddAnimation(400);		// idle right big
	//mario->AddAnimation(401);		// idle left big
	//mario->AddAnimation(402);		// idle right small
	//mario->AddAnimation(403);		// idle left small

	//mario->AddAnimation(500);		// walk right big
	//mario->AddAnimation(501);		// walk left big
	//mario->AddAnimation(502);		// walk right small
	//mario->AddAnimation(503);		// walk left big

	//mario->AddAnimation(599);		// die

	//mario->SetPosition(50.0f, 0);

	simon = new CSimon();
	simon->AddAnimation("simon_idle_left");		// idle left big

	simon->AddAnimation("simon_left");		// walk left big

	simon->SetPosition(0.0f, 0);
	objects.push_back(simon);

	for (int i = 0; i < 5; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(100.0f + i*60.0f, 74.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(100.0f + i*60.0f, 90.0f);
		objects.push_back(brick);

		brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(84.0f + i*60.0f, 90.0f);
		objects.push_back(brick);
	}


	for (int i = 0; i < 30; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation("brick");
		brick->SetPosition(0 + i*16.0f, 300);
		objects.push_back(brick);
	}

	//// and Goombas 
	//for (int i = 0; i < 4; i++)
	//{
	//	goomba = new CGoomba();
	//	goomba->AddAnimation(701);
	//	goomba->AddAnimation(702);
	//	goomba->SetPosition(200 + (float)i*60, 135);
	//	goomba->SetState(GOOMBA_STATE_WALKING);
	//	objects.push_back(goomba);
	//}

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
	}


	// Update camera to follow simon
	//float cx, cy;
	//simon->GetPosition(cx, cy);

	//cx -= SCREEN_WIDTH / 2;
	//cy -= SCREEN_HEIGHT / 2;

	//CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

/*
	Render a frame 
*/
void Render()
{
	auto d3ddv = game->GetDirect3DDevice();
	auto bb = game->GetBackBuffer();
	auto spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

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

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}