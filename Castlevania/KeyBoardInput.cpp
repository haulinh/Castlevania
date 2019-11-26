﻿#include "KeyBoardInput.h"

KeyBoardInput::KeyBoardInput(Game *game, SceneManager *scene)
{
	this->game = game;
	this->scene = scene;
}

KeyBoardInput::~KeyBoardInput()
{
}

void KeyBoardInput::KeyState(BYTE *state)
{
	if (scene->GetSimon()->IsJumping())
		return;

	if (scene->GetSimon()->IsStandAttacking())
		return;

	if (scene->GetSimon()->IsSitAttacking())
		return;

	if (scene->GetSimon()->IsThrowing())
		return;

	if (scene->GetSimon()->IsPowering())
		return;

	else if (game->IsKeyDown(DIK_DOWN))
		scene->GetSimon()->SetState(Sit);

	else if (game->IsKeyDown(DIK_RIGHT))
	{
		scene->GetSimon()->SetN(1);
		scene->GetSimon()->SetState(Walking);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		scene->GetSimon()->SetN(-1);
		scene->GetSimon()->SetState(Walking);
	}

	else
		scene->GetSimon()->SetState(Idle);
}

void KeyBoardInput::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (scene->GetSimon()->GetState() == Jump || scene->GetSimon()->GetState() == StandAttack || scene->GetSimon()->GetState() == SitAttack)
			return;
		scene->GetSimon()->SetState(Jump);
		break;

	case DIK_D:
		if ((scene->GetSimon()->GetState() == StandAttack || scene->GetSimon()->GetState() == SitAttack))
			return;
		if (scene->GetSimon()->GetState() == Idle || scene->GetSimon()->GetState() == Jump)
		{
			scene->GetSimon()->SetState(StandAttack);
		}
		else if (scene->GetSimon()->GetState() == Sit)
		{
			scene->GetSimon()->SetState(SitAttack);
		}
		break;

	case DIK_X:
	/*	if (!scene->GetSimon()->isPowered)
			return;*/
		if (scene->GetSimon()->GetState() == Idle || scene->GetSimon()->GetState() == Jump)
		{
			Simon* simon = scene->GetSimon();
			SubWeapon* subweapon = scene->GetSubWeapon();
			float sx, sy;

			// position
			simon->GetPosition(sx, sy);
			subweapon->SetPosition(sx, sy);

			// orientation
			subweapon->SetN(simon->GetN());

			// state weapon
			subweapon->SetState(simon->GetSubWeapon());

			subweapon->SetEnable(true);
			simon->LoseEnergy(1);
			simon->SetState(Throw);
		}
	default:
		break;
	}
}

void KeyBoardInput::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
