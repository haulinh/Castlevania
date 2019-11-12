#include "Sprites.h"
#include "Game.h"
#include "debug.h"

CSprite::CSprite(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	this->idSprite = idSprite;
	this->left= left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->texture = tex;
}

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::Draw(int nx, float x, float y, int alpha)
{
	Game * game = Game::GetInstance();
	game->Draw(nx, x, y, texture, left, top, width, height, alpha);
}

void CSprite::Draw(float x, float y, int alpha)
{
	Game* game = Game::GetInstance();
	game->Draw(x, y, texture, left, top, width, height, alpha);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSprites::Add(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(idSprite, left, top, width, height, tex);
	sprites[idSprite] = s;
}


LPSPRITE CSprites::Get(string idSprite)
{
	return sprites[idSprite];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t=this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
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