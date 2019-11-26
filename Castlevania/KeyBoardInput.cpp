#include "KeyBoardInput.h"

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
		scene->GetSimon()->SetState(SIT);

	else if (game->IsKeyDown(DIK_RIGHT))
	{
		scene->GetSimon()->SetN(1);
		scene->GetSimon()->SetState(WALK);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		scene->GetSimon()->SetN(-1);
		scene->GetSimon()->SetState(WALK);
	}

	else
		scene->GetSimon()->SetState(IDLE);
}

void KeyBoardInput::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (scene->GetSimon()->GetState() == JUMP || scene->GetSimon()->GetState() == STAND_ATTACK || scene->GetSimon()->GetState() == SIT_ATTACK)
			return;
		scene->GetSimon()->SetState(JUMP);
		break;

	case DIK_D:
		if ((scene->GetSimon()->GetState() == STAND_ATTACK || scene->GetSimon()->GetState() == SIT_ATTACK))
			return;
		if (scene->GetSimon()->GetState() == IDLE || scene->GetSimon()->GetState() == JUMP)
		{
			scene->GetSimon()->SetState(STAND_ATTACK);
		}
		else if (scene->GetSimon()->GetState() == SIT)
		{
			scene->GetSimon()->SetState(SIT_ATTACK);
		}
		break;

	case DIK_X:
	/*	if (!scene->GetSimon()->isPowered)
			return;*/
		if (scene->GetSimon()->GetState() == IDLE || scene->GetSimon()->GetState() == JUMP)
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
			simon->SetState(THROW);
		}
	default:
		break;
	}
}

void KeyBoardInput::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
