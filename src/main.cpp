/*-------------------------------------------------------------------------\
| File: MAIN.CPP															|
| Desc: Entry Point for Pinball.exe											|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include <ctime>

void InitLog()
{
	Log::SetWorkingDir(GetCurrentDir());

	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::string s;
	
	s += " ----- Instance Run on " + std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_mday) + "/" +  std::to_string(now->tm_year+1900) + " at " +
		std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + " ----- \n";

	Log::Write(s.c_str(), ENGINE_LOG);
}

int main(int argc, char *argv[])
{
	InitLog();

	Pinball game("Pinball Game", 800, 600, 100, 100);

	game.Run(argc, argv);

	return 0;
}