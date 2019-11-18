#pragma once

#include "Game.h"
#include "Simon.h"

class KeyBoardInput : public KeyEventHandler
{
	Game* game;
	Simon* simon;
public:
	KeyBoardInput(Game* game, Simon* simon);
	~KeyBoardInput();

	virtual void KeyState(BYTE* state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

