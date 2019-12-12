#include "KeyBoardInput.h"

KeyBoardInput::KeyBoardInput(Game* game, SceneManager* scene)
{
	this->game = game;
	this->scene = scene;
}

KeyBoardInput::~KeyBoardInput()
{
}

void KeyBoardInput::KeyState(BYTE* state)
{
	bool isCollideWithStair = false;

	Simon* simon = scene->GetSimon();
	vector<LPGAMEOBJECT>* listStairs = scene->GetListStairs();

	simon->CheckCollisionWithStair(listStairs);

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

	if (scene->GetSimon()->IsDeflecting())
		return;

	if (scene->GetSimon()->IsAutoWalk() == true)
		return;

	if (scene->GetSimon()->GetState() == STAIR_UP && scene->GetSimon()->animations[STAIR_UP]->IsOver(200) == false)
		return;

	if (scene->GetSimon()->GetState() == STAIR_DOWN && scene->GetSimon()->animations[STAIR_DOWN]->IsOver(200) == false)
		return;

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

	else if (game->IsKeyDown(DIK_DOWN))
	{
		string prevState = simon->GetState();

		if (simon->notSit == true)
		{
			if (simon->IsMovingDown() == false)
			{
				simon->SetState(IDLE);

				return;
			}


			simon->SetN(-simon->GetStairDirection());
			simon->SetState(STAIR_DOWN);


			if (simon->IsStandOnStair() == false)
			{
				simon->SetStandOnStair(true);
				simon->PositionCorrection();
			}
			else if (prevState == STAIR_UP)
			{
				simon->PositionCorrection(prevState);
			}
			return;
		}

		scene->GetSimon()->SetState(SIT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		string prevState = simon->GetState();

		if (simon->notSit)
		{
			if (simon->IsMovingUp() == false)
			{
				if (prevState == STAIR_UP || prevState == STAIR_DOWN)
				{
					int nx = simon->GetN();
					simon->SetState(STAIR_UP);
					simon->AutoWalk(16 * nx, IDLE, nx);
				}

				return;
			}


			simon->SetN(simon->GetStairDirection());
			simon->SetState(STAIR_UP);


			if (simon->IsStandOnStair() == false)
			{
				simon->SetStandOnStair(true);
				simon->PositionCorrection();
			}
			else if (prevState == STAIR_DOWN)
			{
				simon->PositionCorrection(prevState);
			}
		}
		else
		{
			simon->SetState(IDLE);
		}
	}
	else
	{
		if (simon->GetState() == STAIR_UP || simon->GetState() == STAIR_DOWN)
		{
			simon->StandOnStair();
			simon->animations[simon->GetState()]->Reset();
			return;
		}

		scene->GetSimon()->SetState(IDLE);
	}
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
