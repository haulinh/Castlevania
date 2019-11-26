#include "LoadResourceFile.h"
#include <rapidxml\rapidxml_utils.hpp>
#include "Sprites.h"
#include "Textures.h"

LoadResourceFile* LoadResourceFile::_instance = NULL;

LoadResourceFile* LoadResourceFile::GetInstance()
{
	if (_instance == NULL)
		_instance = new LoadResourceFile();
	return _instance;
}

void LoadResourceFile::LoadSpriteSheetFile(const char* filePath, LPDIRECT3DTEXTURE9 tex)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("TextureAtlas");
	for (xml_node<>* spriteNode = rootNode->first_node(); spriteNode; spriteNode = spriteNode->next_sibling())
	{

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

		Sprites* sprites = Sprites::GetInstance();
		sprites->Add(idSprite, left, top, width, height, tex);
	}
}

void LoadResourceFile::LoadAnimationsFile(const char* filePath)
{
	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");
	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling())
	{

		LPANIMATION ani;

		int defaultTime = atoi(animationNode->first_attribute("defaultTime")->value());
		ani = new Animation(defaultTime);

		xml_node<>* frameNode = rootNode->first_node("frame");
		for (xml_node<>* frameNode = animationNode->first_node("frame"); frameNode; frameNode = frameNode->next_sibling())
		{
			string spriteId = string(frameNode->first_attribute("spriteID")->value());
			int time = atoi(frameNode->first_attribute("time")->value());
			ani->Add(spriteId, time);
		}

		string aniId = string(animationNode->first_attribute("ID")->value());
		Animations* animations = Animations::GetInstance();
		animations->Add(aniId, ani);
	}
}

vector<string> LoadResourceFile::GetAnimations(const char* filePath)
{
	vector<string> animationsList;

	rapidxml::file<> xmlFile(filePath);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("animations");
	int i = 0;
	for (xml_node<>* animationNode = rootNode->first_node(); animationNode; animationNode = animationNode->next_sibling())
	{

		string aniId = string(animationNode->first_attribute("ID")->value());
		animationsList.push_back(aniId);
	}

	return animationsList;
}

void LoadResourceFile::LoadTextures()
{
	Textures* textures = Textures::GetInstance();

	textures->Add(ID_BBOX, L"resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	textures->Add(ID_TEX_SIMON, L"resources\\simon\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_GROUND, L"resources\\ground\\ground.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_WEAPON, L"resources\\weapons\\weapons.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_CANDLE, L"resources\\candle\\candle.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ITEMS, L"resources\\items\\items.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFECT, L"resources\\effect\\effect.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_SUB_WEAPONS, L"resources\\sub_weapons\\sub_weapons.png", D3DCOLOR_XRGB(255, 0, 255));
}

void LoadResourceFile::LoadAllResource()
{
	Textures* textures = Textures::GetInstance();

	LoadTextures();
	auto texGround = textures->Get(ID_TEX_GROUND);
	auto texSimon = textures->Get(ID_TEX_SIMON);
	auto texWeapon = textures->Get(ID_TEX_WEAPON);
	auto texCandle = textures->Get(ID_TEX_CANDLE);
	auto texItems = textures->Get(ID_TEX_ITEMS);
	auto texEffect = textures->Get(ID_TEX_EFFECT);
	auto texSubWeapons = textures->Get(ID_TEX_SUB_WEAPONS);

	LoadSpriteSheetFile("resources\\ground\\ground.xml", texGround);
	LoadSpriteSheetFile("resources\\simon\\simon.xml", texSimon);
	LoadSpriteSheetFile("resources\\weapons\\weapons.xml", texWeapon);
	LoadSpriteSheetFile("resources\\candle\\candle.xml", texCandle);
	LoadSpriteSheetFile("resources\\items\\items.xml", texItems);
	LoadSpriteSheetFile("resources\\effect\\effect.xml", texEffect);
	LoadSpriteSheetFile("resources\\sub_weapons\\sub_weapons.xml", texSubWeapons);

	LoadAnimationsFile("resources\\simon\\simon_ani.xml");
	LoadAnimationsFile("resources\\weapons\\weapons_ani.xml");
	LoadAnimationsFile("resources\\ground\\ground_ani.xml");
	LoadAnimationsFile("resources\\candle\\candle_ani.xml");
	LoadAnimationsFile("resources\\items\\items_ani.xml");
	LoadAnimationsFile("resources\\effect\\effect_ani.xml");
	LoadAnimationsFile("resources\\sub_weapons\\sub_weapons_ani.xml");
}
