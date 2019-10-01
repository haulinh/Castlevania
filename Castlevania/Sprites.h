#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

using namespace rapidxml;

using namespace std;

class CSprite
{
	string idSprite;				// Sprite ID in the sprite database

	int left; 
	int top;
	int width;
	int height;

	LPDIRECT3DTEXTURE9 texture;
public: 
	CSprite(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex);

	void Draw(int nx, float x, float y, int alpha = 255);
	void Draw(float x, float y, int alpha = 255);
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<string, LPSPRITE> sprites;

public:
	void Add(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex);
	void LoadSpriteSheet(const char* filePath, LPDIRECT3DTEXTURE9 tex);
	LPSPRITE Get(string idSprite);
	LPSPRITE &operator[](string idSprite) {return sprites[idSprite];}

	static CSprites * GetInstance();
};

/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(string spriteId, DWORD time = 0);
	void Render(int nx, float x, float y, int alpha=255);
	void Render(float x, float y, int alpha = 255);
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<string, LPANIMATION> animations;

public:
	void Add(string idAni, LPANIMATION ani);
	LPANIMATION Get(string idAni);

	static CAnimations * GetInstance();
};

