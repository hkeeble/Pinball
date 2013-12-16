#ifndef _GAME_DATA_H_
#define _GAME_DATA_H_

#include <fstream>
#include <iostream>
#include "monitor.h"

using namespace std;

struct GameData
{
private:
	void cpy(const GameData& param);
public:
	BallData* balls;
	int avgDurationPerBall, avgScorePerBall, avgScorePerSecond, totalDuration, totalScore, numberOfBalls;

	GameData();
	~GameData();
	GameData(const GameData& param);
	GameData& operator=(const GameData& param);

	void ReadData(string fileName);
	void OutputData();
	GameData Difference(GameData comparee);
};

#endif // _GAME_DATA_H_