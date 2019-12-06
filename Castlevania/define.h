#pragma once
#include <string>
using namespace std;

#pragma region define window

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE 200

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

// define id texture
enum TexId
{
	SCENE_1,
	SCENE_2,
	SCENE_3,
	ID_BBOX,
	ID_TEX_SIMON,
	ID_TEX_WEAPON,
	ID_TEX_GROUND,
	ID_TEX_CANDLE,
	ID_TEX_ITEMS,
	ID_TEX_EFFECT,
	ID_TEX_HP,
	ID_TEX_SUB_WEAPONS,
	ID_TEX_STAIR,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define State

// Simon
const string IDLE = "SIMON_IDLE";
const string WALK = "SIMON_WALKING";
const string JUMP = "SIMON_JUMPING";
const string SIT = "SIMON_SITTING";
const string STAND_ATTACK = "SIMON_STAND_ATTACKING";
const string SIT_ATTACK = "SIMON_SIT_ATTACKING";
const string THROW = "SIMON_THROWING";
const string DIE = "SIMON_DIE";
const string POWER = "SIMON_POWERING";
const string STAIR_UP = "SIMON_STAIR_UP";
const string STAIR_DOWN = "SIMON_STAIR_DOWN";

// Weapon
const string MAGIC_WHIP = "MAGIC_WHIP";
const string SHORT_CHAIN = "SHORT_CHAIN";
const string LONG_CHAIN = "LONG_CHAIN";

// Candle
const string LARGE_CANDLE = "LARGE_CANDLE";
const string DESTROY = "DESTROY";

// Ground
const string GROUND_1 = "GROUND_1";
const string GROUND_2 = "GROUND_2";

// Items
const string STOP_WATCH = "STOP_WATCH";
const string DAGGER = "DAGGER";
const string AXE = "AXE";
const string HOLY_WATER = "HOLY_WATER";
const string BOOMERANG = "BOOMERANG";
const string SMALL_HEART = "SMALL_HEART";
const string LARGE_HEART = "LARGE_HEART";
const string CROSS = "CROSS";
const string INVISIBILITY_POTION = "INVISIBILITY_POTION";
const string CHAIN = "CHAIN";
const string MONEY_BAG_RED = "MONEY_BAG_RED";
const string MONEY_BAG_BLUE = "MONEY_BAG_BLUE";
const string MONEY_BAG_WHITE = "MONEY_BAG_WHITE";
const string MONEY_BAG_FLASHING = "MONEY_BAG_FLASHING";
const string DOUBLE_SHOT = "DOUBLE_SHOT";
const string TRIPLE_SHOT = "TRIPLE_SHOT";
const string PORK_CHOP = "PORK_CHOP";
const string MAGIC_CRYSTAL = "MAGIC_CRYSTAL";

// Sub weapons
const string DAGGER_SUB = "DAGGER_SUB";
const string AXE_SUB = "AXE_SUB";
const string BOOMERANG_SUB = "BOOMERANG_SUB";
const string HOLY_WATER_SUB = "HOLYWATER_SUB";
const string STOP_WATCH_SUB = "STOP_WATCH_SUB";

// Stair
const string STAIR_LEFT_UP = "STAIR_LEFT_UP";
const string STAIR_RIGHT_DOWN = "STAIR_RIGHT_DOWN";

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define Properties

// Simon
constexpr float SIMON_WALKING_SPEED = 0.1f;
constexpr float SIMON_JUMP_SPEED_Y = 0.5f;
constexpr float SIMON_GRAVITY = 0.002f;
constexpr float SIMON_GRAVITY_LOWER = 0.001f;
constexpr float SIMON_STAIR_SPEED_X = 0.08f;
constexpr float SIMON_STAIR_SPEED_Y = 0.08f;

// Item
constexpr float ITEM_FALLING_SPEED = 0.1f;
constexpr float ITEM_TIME_DESTROYED = 100000;
constexpr float ITEM_DAGGER_SPEED = 0.3f;

// SubWeapon
constexpr float WEAPONS_DAGGER_SPEED = 0.3f;

constexpr float WEAPONS_AXE_SPEED_X = 0.2f;
constexpr float WEAPONS_AXE_SPEED_Y = 0.5f;

constexpr float WEAPONS_HOLY_WATER_SPEED_X = 0.25f;
constexpr float WEAPONS_HOLY_WATER_SPEED_Y = 0.1f;

constexpr float WEAPONS_BOOMERANG_SPEED = 0.5f;

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
#define LARGE_CANDLE_BBOX_WIDTH			32
#define LARGE_CANDLE_BBOX_HEIGHT			64

#define CANDLE_BBOX_WIDTH		16
#define CANDLE_BBOX_HEIGHT	32

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

// Stair
#define STAIR_BBOX_WIDTH			32
#define STAIR_BBOX_HEIGHT			32

#pragma endregion

#pragma region ID objects to load from file

const string CANDLE = "CANDLE";
const string GROUND= "GROUND";
const string STAIR = "STAIR";

#pragma endregion
