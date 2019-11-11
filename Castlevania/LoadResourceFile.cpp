#include "LoadResourceFile.h"
#include <rapidxml\rapidxml_utils.hpp>
#include "Sprites.h"
#include "Textures.h"

LoadResourceFile* LoadResourceFile::_instance = NULL;

LoadResourceFile* LoadResourceFile::GetInstance()
{
	if (_instance == NULL) _instance = new LoadResourceFile();
	return _instance;
}

void LoadResourceFile::LoadSpriteSheetFile(const char* filePath, LPDIRECT3DTEXTURE9 tex)
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

		CSprites* sprites = CSprites::GetInstance();
		sprites->Add(idSprite, left, top, width, height, tex);
	}
}

void LoadResourceFile::LoadAnimationsFile(const char* filePath)
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
			int time = atoi(frameNode->first_attribute("time")->value());
			ani->Add(spriteId, time);
		}

		string aniId = string(animationNode->first_attribute("ID")->value());
		CAnimations* animations = CAnimations::GetInstance();
		animations->Add(aniId, ani);
	}
}

void LoadResourceFile::LoadTextures()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(id_tex_simon, L"resources\\simon\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(id_tex_ground, L"resources\\ground\\ground.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(id_tex_whip, L"resources\\whip\\whip.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(id_tex_Candle, L"resources\\Candle\\Candle.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(-100, L"resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
}

vector<string> LoadResourceFile::GetAnimations(const char* filePath)
{
	vector<string> animationsList;

	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");
	//xml_node<>* animationNode = rootNode->first_node("animation");
	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling()) {

		string aniId = string(animationNode->first_attribute("ID")->value());
		animationsList.push_back(aniId);
	}

	return animationsList;
}

void LoadResourceFile::LoadAllResource()
{
	CTextures* textures = CTextures::GetInstance();
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	LoadResourceFile* LoadResourceFile = LoadResourceFile::GetInstance();

	LoadResourceFile->LoadTextures();
	auto texGround= textures->Get(id_tex_ground);
	auto texSimon = textures->Get(id_tex_simon);
	auto texWhip = textures->Get(id_tex_whip);
	auto texCandle = textures->Get(id_tex_Candle);


	LoadResourceFile->LoadSpriteSheetFile("resources\\ground\\ground.xml", texGround);
	LoadResourceFile->LoadSpriteSheetFile("resources\\simon\\simon.xml", texSimon);
	LoadResourceFile->LoadSpriteSheetFile("resources\\whip\\whip.xml", texWhip);
	LoadResourceFile->LoadSpriteSheetFile("resources\\Candle\\Candle.xml", texCandle);


	LoadResourceFile->LoadAnimationsFile("resources\\simon\\simon_ani.xml");
	LoadResourceFile->LoadAnimationsFile("resources\\whip\\whip_ani.xml");
	LoadResourceFile->LoadAnimationsFile("resources\\ground\\ground_ani.xml");
	LoadResourceFile->LoadAnimationsFile("resources\\Candle\\Candle_ani.xml");

}
