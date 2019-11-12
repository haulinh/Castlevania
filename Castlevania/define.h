#pragma once
#include <string>
using namespace std;

#pragma region define window

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(25, 25, 25)
#define SCREEN_WIDTH 640	 
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE  60

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define texture

enum TexId
{
	id_bbox,
	id_tex_simon,
	id_tex_weapon,
	id_tex_ground,
	id_tex_candle,
	id_tex_items,
	id_tex_effect,
};

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define simon

constexpr float simon_walking_speed = 0.1f;
constexpr float simon_jump_speed_y = 0.5f;
constexpr float simon_jump_deflect_speed = 0.2f;
constexpr float simon_gravity = 0.002f;
constexpr float simon_die_deflect_speed = 0.5f;

enum Candlestate
{
	candle,
	Candle_delete,
};

// state
const string Idle = "simon_ani_idle";
const string Walking= "simon_ani_walking";
const string Jump = "simon_ani_jumping";
const string Sit = "simon_ani_sitting";
const string Attack = "simon_ani_attacking";
const string Throw = "simon_ani_throwing";
const string Die = "simon_ani_die";
const string Power = "simon_ani_powering";

const string MagicWhip = "magic_whip";
const string ShortChain= "short_chain";
const string LongChain= "long_chain";

const string LargeCandle = "large_candle";
const string Destroy = "destroy";


// Simon
#define SIMON_BBOX_WIDTH			40
#define SIMON_BBOX_HEIGHT			62

#define Candle_BBOX_WIDTH	32
#define Candle_BBOX_HEIGHT	64

#define TILEMAP1_WIDTH 1536

// Weapon
#define Weapon_BBOX_WIDTH				40
#define Weapon_BBOX_HEIGHT			10

// Items
const string LARGE_HEART = "LargeHeart";
const string CHAIN = "Chain";
const string DAGGER = "Dagger";

// Large Heart
#define LARGE_HEART_BBOX_WIDTH		24
#define LARGE_HEART_BBOX_HEIGHT		20

// Chain
#define CHAIN_BBOX_WIDTH			32
#define CHAIN_BBOX_HEIGHT			32

// Dagger
#define DAGGER_BBOX_WIDTH			32
#define DAGGER_BBOX_HEIGHT			18

// Item
#define ITEM_FALLING_SPEED			0.2f
#define ITEM_TIME_DESTROYED			3000
#define ITEM_DAGGER_SPEED			0.3f

#pragma endregion

