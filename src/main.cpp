/*-------------------------------------------------------------------------\
| File: MAIN.CPP															|
| Desc: Entry Point for Pinball.exe											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"

int main(int argc, char *argv[])
{
	Pinball game("Pinball Game", 800, 600, 100, 100);

	game.Run(argc, argv);

	return 0;
}