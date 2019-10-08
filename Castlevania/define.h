#pragma once
#include <string>
using namespace std;

#pragma region define window

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(25, 25, 25)
#define SCREEN_WIDTH 578	
#define SCREEN_HEIGHT 447

#define MAX_FRAME_RATE  60

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region define texture

enum TexId
{
	id_tex_simon,
	id_tex_whip,
	id_tex_ground,
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

enum SimonState
{
	simon_state_idle,
	simon_state_walking_right,
	simon_state_walking_left,
	simon_state_jump,
	simon_state_die,
	simon_state_sit,
	simon_state_attack,
};


#define SIMON_BIG_BBOX_WIDTH 60 
#define SIMON_BIG_BBOX_HEIGHT 63 

#define SIMON_UNTOUCHABLE_TIME 5000

#pragma endregion

#define isEnable 100
//enum BrickState
//{
//	destroy,
//};
