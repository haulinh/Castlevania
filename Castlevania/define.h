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
	id_tex_simon,
	id_tex_whip,
	id_tex_ground,
	id_tex_Candle,
	id_bbox,
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

const string Idle = "simon_ani_idle";
const string Walking= "simon_ani_walking";
const string Jump = "simon_ani_jumping";
const string Sit = "simon_ani_sitting";
const string Attack = "simon_ani_attacking";
const string Die = "simon_ani_die";


#define SIMON_BIG_BBOX_WIDTH 30
#define SIMON_BIG_BBOX_HEIGHT 60 
#define Candle_BBOX_WIDTH	32
#define Candle_BBOX_HEIGHT	64

#define TILEMAP1_WIDTH 1536
#define SIMON_UNTOUCHABLE_TIME 5000

#pragma endregion

#define isEnable 100
