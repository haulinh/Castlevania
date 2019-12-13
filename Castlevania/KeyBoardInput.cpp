#include "KeyBoardInput.h"
#include "define.h"

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
	Simon* simon = scene->GetSimon();

	if (simon->IsAutoWalk() == true)
		return;

	if (isNeedToWaitingAnimation == true)
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

		if (scene->GetSimon()->IsDeflecting())
			return;

		if (scene->GetSimon()->IsAutoWalk() == true)
			return;

		if (scene->GetSimon()->GetState() == STAIR_UP && scene->GetSimon()->animations[STAIR_UP]->IsOver(200) == false)
			return;

		if (scene->GetSimon()->GetState() == STAIR_DOWN && scene->GetSimon()->animations[STAIR_DOWN]->IsOver(200) == false)
			return;

		if (simon->GetState() == STAIR_UP_ATTACK && simon->animations[STAIR_UP_ATTACK]->IsOver(300) == false)
			return;

		if (simon->GetState() == STAIR_DOWN_ATTACK && simon->animations[STAIR_DOWN_ATTACK]->IsOver(300) == false)
			return;

	}
	else
	{
		// Đặt lại biến chờ render animation
		isNeedToWaitingAnimation = true;

		// Để tránh việc ở frame tiếp theo rơi vào trạng thái chờ render animation 
		// (vì animation == 200ms, một frame == 30ms nên sẽ phải bị chờ dù cho có biến isNeedToWaitingAnimation),
		// do đó cần reset lại animation start time về 0
		simon->animations[simon->GetState()]->SetAniStartTime(0);
	}


	// Xét trạng thái phím
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (StairCollisionsDetection() == true && simon->IsStandOnStair() == true)
		{
			if (simon->GetStairDirection() == 1) // cầu thang trái dưới - phải trên
			{
				Simon_Stair_Up();
			}
			else {
				Simon_Stair_Down();
			}

			return;
		}

		Simon_Walk_Right();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (StairCollisionsDetection() == true && simon->IsStandOnStair() == true)
		{
			if (simon->GetStairDirection() == 1) // cầu thang trái dưới - phải trên
			{
				Simon_Stair_Down();
			}
			else {
				Simon_Stair_Up();
			}

			return;
		}

		Simon_Walk_Left();
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (StairCollisionsDetection() == true)
		{
			Simon_Stair_Down();
			return;
		}

		simon->SetState(SIT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (StairCollisionsDetection() == true)
		{
			Simon_Stair_Up();
			return;
		}

		simon->SetState(IDLE);
	}
	else
	{
		simon->SetHitSubWeapons(false);
		if (StairCollisionsDetection() == true)
		{
			if (Simon_Stair_Stand() == true)
			{
				return;
			}
		}

		simon->SetState(IDLE);
	}
}

void KeyBoardInput::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_SPACE:
		Simon_Jump();
		break;
	case DIK_D:
		if (game->IsKeyDown(DIK_UP))
			Simon_Hit_SubWeapon();
		else
			Simon_Hit();
		break;
	case DIK_X:
		Simon_Hit_SubWeapon();
		break;
	case DIK_0:
		scene->GetSimon()->SetSubWeapon(DAGGER_SUB);
		break;
	case DIK_1:
		scene->GetSimon()->SetSubWeapon(AXE_SUB);
		break;
	case DIK_2:
		scene->GetSimon()->SetSubWeapon(BOOMERANG_SUB);
		break;
	case DIK_3:
		scene->GetSimon()->SetSubWeapon(HOLY_WATER_SUB);
		break;
	case DIK_4:
		scene->GetSimon()->SetSubWeapon(STOP_WATCH_SUB);
		break;
	default:
		break;
	}
}

void KeyBoardInput::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
}

void KeyBoardInput::Simon_Walk_Left()
{
	scene->GetSimon()->SetN(-1);
	scene->GetSimon()->SetState(WALK);
}

void KeyBoardInput::Simon_Walk_Right()
{
	scene->GetSimon()->SetN(1);
	scene->GetSimon()->SetState(WALK);
}

void KeyBoardInput::Simon_Jump()
{
	if (scene->GetSimon()->GetState() == JUMP ||
		scene->GetSimon()->GetState() == STAND_ATTACK ||
		scene->GetSimon()->GetState() == SIT_ATTACK)
		return;

	scene->GetSimon()->SetState(JUMP);
}

void KeyBoardInput::Simon_Hit()
{
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
	else if (scene->GetSimon()->GetState() == STAIR_UP)
	{
		scene->GetSimon()->SetState(STAIR_UP_ATTACK);
	}
	else if (scene->GetSimon()->GetState() == STAIR_DOWN)
	{
		scene->GetSimon()->SetState(STAIR_DOWN_ATTACK);
	}
}

void KeyBoardInput::Simon_Hit_SubWeapon()
{
	Simon* simon = scene->GetSimon();
	SubWeapon* subweapon = scene->GetSubWeapon();

	if(subweapon->IsEnable() == true) // đang phóng rồi
		return;

	if (simon->GetState() == IDLE || simon->GetState() == JUMP ||
		simon->GetState() == SIT || simon->GetState() == STAIR_UP ||
		simon->GetState() == STAIR_DOWN)
	{
		float sx, sy;

		// position
		simon->GetPosition(sx, sy);

		if (simon->GetState() == SIT) sy += 25.0f; // khớp vị trí tay
		else sy += 10.0f;
		if (simon->GetN() < 0) sx += 30.0f;
		else sx += 30.0f;

		subweapon->SetPosition(sx, sy);

		// orientation
		subweapon->SetN(simon->GetN());

		// state weapon
		subweapon->SetState(simon->GetSubWeapon());
		subweapon->SetEnable(true);

		if (subweapon->GetState() == STOP_WATCH_SUB)
		{
			/*	simon->LoseEnergy(5);
				scene->StartStopWatch();*/
		}
		else
		{
			//simon->LoseEnergy(1);
			simon->SetHitSubWeapons(true);
			Simon_Hit();
		}
	}
}

void KeyBoardInput::Simon_Stair_Down()
{
	Simon* simon = scene->GetSimon();
	string prevState = simon->GetState();
	int stairDirection = simon->GetStairDirection();

	if (simon->IsMovingDown() == false)
	{
		simon->SetState(IDLE);

		return;
	}


	// Auto-walk của Simon đi đến đúng đầu cầu thang rồi mới bước lên
	if (simon->IsStandOnStair() == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == -1) stair_x -= 28.0f;

		if (stair_x < simon_x) simon->SetN(-1);
		else if (stair_x > simon_x) simon->SetN(1);
		else return;

		simon->SetState(WALK);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, STAIR_DOWN, -stairDirection);
		simon->SetStandOnStair(true);

		return;
	}
	else
	{
		simon->SetN(-simon->GetStairDirection());
		simon->SetState(STAIR_DOWN);
	}
	return;
}

void KeyBoardInput::Simon_Stair_Up()
{
	Simon* simon = scene->GetSimon();
	string prevState = simon->GetState();
	int stairDirection = simon->GetStairDirection();

	if (simon->IsMovingUp() == false)
	{
		if (prevState == STAIR_UP || prevState == STAIR_DOWN)
		{
			int nx = simon->GetStairDirection();
			simon->SetN(nx);
			simon->SetState(STAIR_UP);
			simon->AutoWalk(16 * nx, IDLE, nx);
		}

		return;
	}

	// Auto-walk của Simon đi đến đúng chân cầu thang rồi mới bước lên
	if (simon->IsStandOnStair() == false)
	{
		float stair_x, simon_x, temp_y;

		simon->GetStairCollided()->GetPosition(stair_x, temp_y);
		simon->GetPosition(simon_x, temp_y);

		if (stairDirection == 1) stair_x -= 31.0f;
		else stair_x += 5.0f;

		if (stair_x < simon_x) simon->SetN(-1);
		else if (stair_x > simon_x)  simon->SetN(1);
		else return;

		simon->SetState(WALK);
		simon->vy = 0;
		simon->AutoWalk(stair_x - simon_x, STAIR_UP, stairDirection);
		simon->SetStandOnStair(true);

		return;
	}
	else
	{
		simon->SetN(stairDirection);
		simon->SetState(STAIR_UP);
	}

	return;
}

bool KeyBoardInput::Simon_Stair_Stand()
{
	Simon* simon = scene->GetSimon();

	if (simon->GetState() == STAIR_UP || simon->GetState() == STAIR_DOWN ||
		simon->GetState() == STAIR_UP_ATTACK || simon->GetState() == STAIR_DOWN_ATTACK)
	{
		if (simon->GetState() == STAIR_UP_ATTACK)
		{
			simon->SetState(STAIR_UP);
			isNeedToWaitingAnimation = false;
		}
		else if (simon->GetState() == STAIR_DOWN_ATTACK)
		{
			simon->SetState(STAIR_DOWN);
			isNeedToWaitingAnimation = false;
		}

		simon->StandOnStair();
		simon->animations[simon->GetState()]->Reset();

		return true;
	}

	return false;
}

bool KeyBoardInput::StairCollisionsDetection()
{
	Simon* simon = scene->GetSimon();
	vector<LPGAMEOBJECT>* listStairs = scene->GetListStairs();

	simon->CheckCollisionWithStair(listStairs);
	return simon->isCollisionWithStair;
}
