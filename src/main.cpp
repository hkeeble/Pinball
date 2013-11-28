/*-------------------------------------------------------------------------\
| File: MAIN.CPP															|
| Desc: Entry Point for Pinball.exe											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include <ctime>

const int WIN_WIDTH = 700;
const int WIN_HEIGHT = 700;

void InitLog()
{
	Log::SetWorkingDir(GetCurrentDir());

	time_t* t = new time_t();
	struct tm* now = new tm();

	*t = time(0);
	localtime_s(now, t);

	std::string s;
	s += " ----- Instance Run on " + std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_mday) + "/" +  std::to_string(now->tm_year+1900) + " at " +
		std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + " ----- \n";

	Log::Write(s.c_str(), ENGINE_LOG);
	delete t;
	delete now;
}

int main(int argc, char *argv[])
{
	InitLog();

	Pinball game("Henri Keeble - KEE09195812 - CMP3001M - Assessment Item 1 - Pinball", WIN_WIDTH, WIN_HEIGHT);

	game.Run(argc, argv);

	return 0;
}