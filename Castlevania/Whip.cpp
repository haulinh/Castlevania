#include "Whip.h"

Whip::Whip()
{
}

void Whip::Render()
{
	animations["whip"]->Render(nx, x, y);
}

void Whip::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
