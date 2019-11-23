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
#define FILEPATH_FONT				L"Font\\prstart.ttf"

// Objects
#define FILEPATH_TEX_BBOX			L"Textures\\BBox.png"
#define FILEPATH_TEX_SIMON			L"Textures\\Simon.png"
#define FILEPATH_TEX_WHIP			L"Textures\\Whip.png"
#define FILEPATH_TEX_CANDLE			L"Textures\\Candle.png"
#define FILEPATH_TEX_GROUND			L"Textures\\Ground.png"
#define FILEPATH_TEX_EFFECT			L"Textures\\Effect.png"

// Items
#define FILEPATH_TEX_ITEM_STOP_WATCH			L"Textures\\Items\\Stopwatch.png"
#define FILEPATH_TEX_ITEM_DAGGER				L"Textures\\Items\\Dagger.png"
#define FILEPATH_TEX_ITEM_AXE					L"Textures\\Items\\Axe.png"
#define FILEPATH_TEX_ITEM_HOLY_WATER			L"Textures\\Items\\HolyWater.png"
#define FILEPATH_TEX_ITEM_BOOMERANG				L"Textures\\Items\\Boomerang.png"
#define FILEPATH_TEX_ITEM_SMALL_HEART			L"Textures\\Items\\SmallHeart.png"
#define FILEPATH_TEX_ITEM_LARGE_HEART			L"Textures\\Items\\LargeHeart.png"
#define FILEPATH_TEX_ITEM_CROSS					L"Textures\\Items\\Cross.png"
#define FILEPATH_TEX_ITEM_INVISIBILITY_POTION	L"Textures\\Items\\InvisibilityPotion.png"
#define FILEPATH_TEX_ITEM_CHAIN					L"Textures\\Items\\Chain.png"
#define FILEPATH_TEX_ITEM_MONEY_BAG				L"Textures\\Items\\MoneyBag.png"
#define FILEPATH_TEX_ITEM_DOUBLE_SHOT			L"Textures\\Items\\DoubleShot.png"
#define FILEPATH_TEX_ITEM_TRIPLE_SHOT			L"Textures\\Items\\TripleShot.png"
#define FILEPATH_TEX_ITEM_PORK_CHOP				L"Textures\\Items\\PorkChop.png"
#define FILEPATH_TEX_ITEM_MAGIC_CRYSTAL			L"Textures\\Items\\MagicCrystal.png"

// Sub Weapons
#define FILEPATH_TEX_WEAPONS_AXE				L"Textures\\SubWeapons\\Axe.png"
#define FILEPATH_TEX_WEAPONS_HOLY_WATER			L"Textures\\SubWeapons\\HolyWater.png"
#define FILEPATH_TEX_WEAPONS_BOOMERANG			L"Textures\\SubWeapons\\Boomerang.png"


// Scenes
#define FILEPATH_TEX_MAP_SCENE_1		L"Scenes\\Scene1.png"
#define FILEPATH_DATA_MAP_SCENE_1		L"Scenes\\Scene1_map.txt"
#define FILEPATH_OBJECTS_SCENE_1		L"Scenes\\Scene1_objects.txt"

#define FILEPATH_TEX_MAP_SCENE_2		L"Scenes\\Scene2.png"
#define FILEPATH_DATA_MAP_SCENE_2		L"Scenes\\Scene2_map.txt"
#define FILEPATH_OBJECTS_SCENE_2		L"Scenes\\Scene2_objects.txt"

// Player
#define FILEPATH_TEX_HP				L"Textures\\HP.png"
#define FILEPATH_TEX_RECT			L"Textures\\Rect.png"

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
	id_tex_hp
};

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define State

// Simon
const string Idle = "simon_ani_idle";
const string Walking = "simon_ani_walking";
const string Jump = "simon_ani_jumping";
const string Sit = "simon_ani_sitting";
const string StandAttack = "simon_ani_stand_attacking";
const string SitAttack = "simon_ani_sit_attacking";
const string Throw = "simon_ani_throwing";
const string Die = "simon_ani_die";
const string Power = "simon_ani_powering";

// Weapon
const string MagicWhip = "magic_whip";
const string ShortChain = "short_chain";
const string LongChain = "long_chain";

// Candle
const string LargeCandle = "LargeCandle";
const string Destroy = "Destroy";

// Items
const string LARGE_HEART = "LargeHeart";
const string CHAIN = "Chain";
const string DAGGER = "Dagger";

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define Properties

// Simon
constexpr float simon_walking_speed = 0.1f;
constexpr float simon_jump_speed_y = 0.5f;
constexpr float simon_gravity = 0.002f;
constexpr float simon_gravity_lower = 0.001f;

// Item
constexpr float ITEM_FALLING_SPEED = 0.1f;
constexpr float ITEM_TIME_DESTROYED = 100000;
constexpr float ITEM_DAGGER_SPEED = 0.3f;

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

#define	CANDLE "Candle"
#define	BRICK "Brick"

#pragma endregion
