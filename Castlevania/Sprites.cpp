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
	CGame * game = CGame::GetInstance();
	game->Draw(nx, x, y, texture, left, top, width, height, alpha);
}

void CSprite::Draw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, texture, left, top, width, height, alpha);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSprites::Add(string idSprite, int left, int top, int width, int height, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new CSprite(idSprite, left, top, width, height, tex);
	sprites[idSprite] = s;
}

void CSprites::LoadSpriteSheet(const char* filePath, LPDIRECT3DTEXTURE9 tex)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("TextureAtlas");
	for (xml_node<>* spriteNode = rootNode->first_node(); spriteNode; spriteNode = spriteNode->next_sibling()) {

		string idSprite;
		int left;
		int top;
		int width;
		int height;

		idSprite = spriteNode->first_attribute("n")->value();
		left = atoi(spriteNode->first_attribute("x")->value());
		top = atoi(spriteNode->first_attribute("y")->value());
		width = atoi(spriteNode->first_attribute("w")->value());
		height = atoi(spriteNode->first_attribute("h")->value());

		DebugOut(L"id = %s left = %d", idSprite, left);

		Add(idSprite, left, top, width, height, tex);
	}
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
	doneCycle = false;
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
				doneCycle = true;
			}
			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}
		
	}
	DebugOut(L"fame %d \n", currentFrame);

	frames[currentFrame]->GetSprite()->Draw(nx, x, y, alpha);
}

void CAnimation::Render(float x, float y, int alpha)
{
	doneCycle = false;
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
			if (currentFrame >= frames.size())
			{
				
				currentFrame = 0;
				doneCycle = true;
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

void CAnimations::LoadAnimations(const char* filePath)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");
	//xml_node<>* animationNode = rootNode->first_node("animation");
	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling()) {

		LPANIMATION ani;

		int defaultTime = atoi(animationNode->first_attribute("defaultTime")->value());
		ani = new CAnimation(defaultTime);

		xml_node<>* frameNode = rootNode->first_node("frame");
		for (xml_node<>* frameNode = animationNode->first_node("frame"); frameNode; frameNode = frameNode->next_sibling())
		{
			string spriteId = string(frameNode->first_attribute("spriteID")->value());
			ani->Add(spriteId);
		}

		string aniId = string(animationNode->first_attribute("ID")->value());
		Add(aniId, ani);
	}
}

LPANIMATION CAnimations::Get(string idAni)
{
	return animations[idAni];
}