#pragma once
#include <string>
using namespace std;

#pragma region define window

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE 120

#pragma endregion

// Time play
#define DEFAULT_TIME_PLAY 300

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region PathFile

// Font
#define FILEPATH_FONT L"Font\\prstart.ttf"

// Scenes
#define FILEPATH_TEX_MAP_SCENE_1 L"Scenes\\Scene1.png"
#define FILEPATH_DATA_MAP_SCENE_1 L"Scenes\\Scene1_map.txt"
#define FILEPATH_OBJECTS_SCENE_1 L"Scenes\\Scene1_objects.txt"

#define FILEPATH_TEX_MAP_SCENE_2 L"Scenes\\Scene2.png"
#define FILEPATH_DATA_MAP_SCENE_2 L"Scenes\\Scene2_map.txt"
#define FILEPATH_OBJECTS_SCENE_2 L"Scenes\\Scene2_objects.txt"

// Player
#define FILEPATH_TEX_HP L"Textures\\HP.png"
#define FILEPATH_TEX_RECT L"Textures\\Rect.png"

#pragma endregion

enum TexId
{
	SCENE_1,
	SCENE_2,
	SCENE_3,
	id_bbox,
	id_tex_simon,
	id_tex_weapon,
	id_tex_ground,
	id_tex_candle,
	id_tex_items,
	id_tex_effect,
	id_tex_hp,
	id_tex_sub_weapons
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define State

// Simon
const string Idle = "SIMON_IDLE";
const string Walking = "SIMON_WALKING";
const string Jump = "SIMON_JUMPING";
const string Sit = "SIMON_SITTING";
const string StandAttack = "SIMON_STAND_ATTACKING";
const string SitAttack = "SIMON_SIT_ATTACKING";
const string Throw = "SIMON_THROWING";
const string Die = "SIMON_DIE";
const string Power = "SIMON_POWERING";

// Weapon
const string MagicWhip = "magic_whip";
const string ShortChain = "short_chain";
const string LongChain = "long_chain";

// Candle
const string LargeCandle = "LargeCandle";
const string Destroy = "Destroy";

 // Items
const string STOP_WATCH = "";
const string DAGGER = "Dagger";
const string AXE = "Axe";
const string HOLY_WATER = "HolyWater";
const string BOOMERANG = "Boomerang";
const string SMALL_HEART = "";
const string LARGE_HEART = "LargeHeart";
const string CROSS = "Cross";
const string INVISIBILITY_POTION = "";
const string CHAIN = "Chain";
const string MONEY_BAG_RED = "";
const string MONEY_BAG_BLUE = "";
const string MONEY_BAG_WHITE = "";
const string MONEY_BAG_FLASHING = "";
const string DOUBLE_SHOT = "";
const string TRIPLE_SHOT = "";
const string PORK_CHOP = "";
const string MAGIC_CRYSTAL = "";

// Sub weapons
const string DAGGER_SUB = "Dagger_Sub";
const string AXE_SUB = "Axe_Sub";
const string BOOMERANG_SUB = "Boomerang_Sub";
const string HOLY_WATER_SUB = "HolyWater_Sub";
const string STOP_WATCH_SUB = "Stop_Watch_Sub";

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define Properties

// Simon
constexpr float SIMON_WALKING_SPEED = 0.1f;
constexpr float SIMON_JUMP_SPEED_Y = 0.5f;
constexpr float SIMON_GRAVITY = 0.002f;
constexpr float SIMON_GRAVITY_LOWER = 0.001f;

// Item
constexpr float ITEM_FALLING_SPEED = 0.1f;
constexpr float ITEM_TIME_DESTROYED = 100000;
constexpr float ITEM_DAGGER_SPEED = 0.3f;

// SubWeapon
const float WEAPONS_DAGGER_SPEED = 0.3f;

const float WEAPONS_AXE_SPEED_X = 0.2f;
const float WEAPONS_AXE_SPEED_Y = 0.5f;

const float WEAPONS_HOLY_WATER_SPEED_X = 0.25f;
const float WEAPONS_HOLY_WATER_SPEED_Y = 0.1f;

const float WEAPONS_BOOMERANG_SPEED = 0.5f;

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BBox Size

// Simon
#define SIMON_BBOX_WIDTH 40
#define SIMON_BBOX_HEIGHT 62

// Ground
#define GROUND_BBOX_WIDTH 32
#define GROUND_BBOX_HEIGHT 32

// Candle
#define CANDLE_BBOX_WIDTH 32
#define CANDLE_BBOX_HEIGHT 64

// Whip
#define LONG_CHAIN_BBOX_WIDTH 80
#define WHIP_BBOX_WIDTH 50
#define WHIP_BBOX_HEIGHT 15

// Large Heart
#define LARGE_HEART_BBOX_WIDTH 24
#define LARGE_HEART_BBOX_HEIGHT 20

// Chain
#define CHAIN_BBOX_WIDTH 32
#define CHAIN_BBOX_HEIGHT 32

// Dagger
#define DAGGER_BBOX_WIDTH 32
#define DAGGER_BBOX_HEIGHT 18

#pragma endregion

#pragma region ID objects to load from file

#define CANDLE "Candle"
#define BRICK "Brick"

#pragma endregion
