#include "KeyBoardInput.h"



KeyBoardInput::KeyBoardInput(Game* game, Simon* simon)
{
	this->game = game;
	this->simon = simon;
}

KeyBoardInput::~KeyBoardInput()
{
}

void KeyBoardInput::KeyState(BYTE* state)
{
	if (simon->IsJumping()) return;

	if (simon->IsStandAttacking()) return;

	if (simon->IsSitAttacking()) return;

	if (simon->IsThrowing()) return;

	if (simon->IsPowering()) return;

	else if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(Sit);

	else if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetN(1);
		simon->SetState(Walking);
	}

	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetN(-1);
		simon->SetState(Walking);
	}

	else
		simon->SetState(Idle);
}

void KeyBoardInput::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->GetState() == Jump || simon->GetState() == StandAttack || simon->GetState() == SitAttack)
			return;
		simon->SetState(Jump);
		break;

	case DIK_D:
		if ((simon->GetState() == StandAttack || simon->GetState() == SitAttack))
			return;
		if (simon->GetState() == Idle || simon->GetState() == Jump)
		{
			simon->SetState(StandAttack);
		}
		else if (simon->GetState() == Sit)
		{
			simon->SetState(SitAttack);
		}
		break;

	case DIK_X:
		if (!simon->isPowered)
			return;
		if (simon->GetState() == Idle || simon->GetState() == Jump)
		{
			/*	float sx, sy;
				simon->GetPosition(sx, sy);
				dagger->SetPosition(sx, sy + 10);
				dagger->nx = simon->nx;
				dagger->isEnable = true;
				simon->SetState(Throw);*/
		}
	default: break;
	}
}

void KeyBoardInput::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}
