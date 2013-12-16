#ifndef _FLIPPERS_H_
#define _FLIPPERS_H_

#include "boardObjects.h"

class Flippers
{
private:
	Flipper *lft, *rgt;
public:
	Flippers(Flipper* left, Flipper* right);
	Flippers(const Flippers& param);
	Flippers& operator=(const Flippers& param);
	~Flippers();

	void FlipLeft();
	void FlipRight();

	Flipper* const GetLeft() const;
	Flipper* const GetRight() const;
};

#endif // _FLIPPERS_H_