#include "flippers.h"

Flippers::Flippers()
{
	lft = nullptr;
	rgt = nullptr;
}

Flippers::Flippers(const Flipper* const right, const Flipper* const left)
{
	lft = (Flipper*)right;
	rgt = (Flipper*)left;
}
Flippers::~Flippers()
{
	if (lft != nullptr)
		delete lft;
	if (rgt != nullptr)
		delete rgt;
}

void Flippers::FlipLeft()
{
	// Flip Left
}

void Flippers::FlipRight()
{
	// Flip Right
}

Flipper* Flippers::GetLeft() const
{
	return lft;
}

Flipper* Flippers::GetRight() const
{
	return rgt;
}