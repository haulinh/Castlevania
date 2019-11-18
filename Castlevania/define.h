#pragma once
#include <string>
using namespace std;

#pragma region define window

#define WINDOW_CLASS_NAME L"Castlevania"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(25, 25, 25)
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480 

#define MAX_FRAME_RATE 120 

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region PathFile

// Scenes
#define FILEPATH_TEX_MAP_SCENE_1		L"Scenes\\Scene1.png"
#define FILEPATH_DATA_MAP_SCENE_1		L"Scenes\\Scene1_map.txt"
#define FILEPATH_OBJECTS_SCENE_1		L"Scenes\\Scene1_objects.txt"

#define FILEPATH_TEX_MAP_SCENE_2		L"Scenes\\Scene2.png"
#define FILEPATH_DATA_MAP_SCENE_2		L"Scenes\\Scene2_map.txt"
#define FILEPATH_OBJECTS_SCENE_2		L"Scenes\\Scene2_objects.txt"


#pragma endregion

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
// Scene
#define SCENE_1		0
#define SCENE_2		1
#define SCENE_3		2

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define simon

//  BBox
#define SIMON_BBOX_WIDTH			40
#define SIMON_BBOX_HEIGHT			62

constexpr float simon_walking_speed = 0.1f;
constexpr float simon_jump_speed_y = 0.5f;
constexpr float simon_gravity = 0.002f;
constexpr float simon_gravity_lower = 0.001f;


// state
const string Idle = "simon_ani_idle";
const string Walking= "simon_ani_walking";
const string Jump = "simon_ani_jumping";
const string Sit = "simon_ani_sitting";
const string StandAttack = "simon_ani_stand_attacking";
const string SitAttack = "simon_ani_sit_attacking";
const string Throw = "simon_ani_throwing";
const string Die = "simon_ani_die";
const string Power = "simon_ani_powering";

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define weapon

const string MagicWhip = "magic_whip";
const string ShortChain= "short_chain";
const string LongChain= "long_chain";

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma  region define items

// Item
#define ITEM_FALLING_SPEED			0.1f
#define ITEM_TIME_DESTROYED			100000
#define ITEM_DAGGER_SPEED			0.3f

// Large Heart
#define LARGE_HEART_BBOX_WIDTH		24
#define LARGE_HEART_BBOX_HEIGHT		20

const string LargeCandle = "0";
const string Destroy = "1";

const string LARGE_HEART = "LargeHeart";
const string CHAIN = "Chain";
const string DAGGER = "Dagger";	

#pragma endregion

#pragma region BBox Size

// Simon
#define SIMON_BBOX_WIDTH			40
#define SIMON_BBOX_HEIGHT			62

// Ground
#define GROUND_BBOX_WIDTH			32
#define GROUND_BBOX_HEIGHT			32

// Candle
#define CANDLE_BBOX_WIDTH			32
#define CANDLE_BBOX_HEIGHT			64

// Whip
#define LONG_CHAIN_BBOX_WIDTH		80
#define WHIP_BBOX_WIDTH				50
#define WHIP_BBOX_HEIGHT			15

// Large Heart
#define LARGE_HEART_BBOX_WIDTH		24
#define LARGE_HEART_BBOX_HEIGHT		20

// Chain
#define CHAIN_BBOX_WIDTH			32
#define CHAIN_BBOX_HEIGHT			32

// Dagger
#define DAGGER_BBOX_WIDTH			32
#define DAGGER_BBOX_HEIGHT			18

#pragma endregion

#pragma region ID objects to load from file

#define	CANDLE		0
#define BRICK	1

#pragma endregion


