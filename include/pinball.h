/*-------------------------------------------------------------------------\
| File: PINBALL.H															|
| Desc: Declarations for a Pinball game (derived from a generic Game class)	|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PINBALL_H
#define PINBALL_H

#include "game.h"
#include <string>

using namespace GameFramework;

class Pinball : public Game
{
	public:
		Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY);
		~Pinball();
};

#endif // PINBALL_H