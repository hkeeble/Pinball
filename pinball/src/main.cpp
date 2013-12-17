/*-------------------------------------------------------------------------\
| File: MAIN.CPP															|
| Desc: Entry Point for Pinball.exe											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"

const int WIN_WIDTH = 700;
const int WIN_HEIGHT = 700;

int main(int argc, char *argv[])
{
	InitLog(ENGINE_LOG);

	Pinball game("Henri Keeble - KEE09195812 - CMP3001M - Assessment Item 1 - Pinball", WIN_WIDTH, WIN_HEIGHT);

	game.Run(argc, argv);

	return 0;
}