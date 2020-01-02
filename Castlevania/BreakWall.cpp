#include "BreakWall.h"

BreakWall::BreakWall()
{
	LoadResourceFile* loadResourceFile = LoadResourceFile::GetInstance();

	vector<string> animationsBreakWall = loadResourceFile->GetAnimations("resources\\break_wall\\break_wall_ani.xml");
	for each (string animation in animationsBreakWall)
	{
		AddAnimation(animation);
	}
}

void BreakWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == BREAK && isBreakToPieces == false)
	{
		isBreakToPieces = true;
		pieces = new WallPieces(x, y);
	}

	if (pieces != NULL)
		pieces->Update(dt);
}

void BreakWall::Render()
{
	if (state == NORMAL)
		return;

	animations[state]->Render(-1, x, y);

	if (isBreakToPieces == true && pieces != NULL)
	{
		if (pieces->GetStartTimeRender() == 0)
			pieces->SetStartTimeRender(GetTickCount());
		else if (GetTickCount() - pieces->GetStartTimeRender() > WALLPIECES_TIME)
		{
			delete pieces;
			pieces = NULL;
			return;
		}

		pieces->Render();
	}
}

void BreakWall::SetState(string state)
{
	GameObject::SetState(state);
}

void BreakWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + GROUND_BBOX_WIDTH;
	bottom = top + GROUND_BBOX_HEIGHT;
}




