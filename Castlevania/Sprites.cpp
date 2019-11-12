#include "Sprites.h"
#include "Game.h"
#include "debug.h"

Sprite::Sprite(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	this->idSprite = idSprite;
	this->left= left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = tex;
}

Sprites * Sprites::__instance = NULL;

Sprites *Sprites::GetInstance()
{
	if (__instance == NULL) __instance = new Sprites();
	return __instance;
}

void Sprite::Draw(int nx, float x, float y, int alpha)
{
	Game * game = Game::GetInstance();
	game->Draw(nx, x, y, texture, left, top, width, height, alpha);
}

void Sprite::Draw(float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(x, y, texture, left, top, width, height, alpha);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sprites::Add(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new Sprite(idSprite, left, top, width, height, tex);
	sprites[idSprite] = s;
}


LPSPRITE Sprites::Get(string idSprite)
{
	return sprites[idSprite];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t=this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(int nx, float x, float y, int alpha)
{
	this->completed = false;
	DWORD now = GetTickCount();
	if (currentFrame == -1) 
	{
		currentFrame = 0; 
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size() - 1)
			{
				completed = true;
			}
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}
		
	}

	frames[currentFrame]->GetSprite()->Draw(nx, x, y, alpha);
}

void CAnimation::Render(float x, float y, int alpha)
{
	this->completed = false;
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size() - 1)
			{
				completed = true;
			}
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(string idAni, LPANIMATION ani)
{
	animations.insert({ idAni, ani });
}

LPANIMATION CAnimations::Get(string idAni)
{
	return animations[idAni];
}