#pragma once

#include "Game.h"
#include "Simon.h"
#include "SceneManager.h"

class KeyBoardInput : public KeyEventHandler
{
	Game* game;
	SceneManager* scene;
public:
	KeyBoardInput(Game* game, SceneManager* scene);
	~KeyBoardInput();

	virtual void KeyState(BYTE* state);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

