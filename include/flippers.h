#ifndef _FLIPPERS_H_
#define _FLIPPERS_H_

#include "boardObjects.h"

class Flippers
{
private:
	Flipper* lft;
	Flipper* rgt;
public:
	Flippers();
	Flippers(const Flipper* const right, const Flipper* const left);
	~Flippers();
	void FlipLeft();
	void FlipRight();
	Flipper* GetLeft() const;
	Flipper* GetRight() const;
};

#endif // _FLIPPERS_H_