#include "gameData.h"
#include "monitor.h"
#include <fstream>

const int MAX_LINE_BUFFER = 256;

GameData::GameData()
{
	balls = nullptr;
	avgDurationPerBall = avgScorePerBall = avgScorePerSecond = totalDuration = totalScore = numberOfBalls = 0;
}

GameData::~GameData()
{
	if (balls)
		delete [] balls;
}

GameData::GameData(const GameData& param)
{
	cpy(param);
}

GameData& GameData::operator=(const GameData& param)
{
	if (this == &param)
		return *this;
	else
	{
		if (balls)
			delete[] balls;
		cpy(param);
		return *this;
	}
}

void GameData::cpy(const GameData& param)
{
	numberOfBalls = param.numberOfBalls;

	if (param.balls)
	{
		balls = new BallData[numberOfBalls];
		for (int i = 0; i < numberOfBalls; i++)
		{
			balls[i].m_duration = param.balls[i].m_duration;
			balls[i].m_score = param.balls[i].m_score;
		}
	}
	else
		balls = nullptr;

	avgDurationPerBall = param.avgDurationPerBall;
	avgScorePerBall = param.avgScorePerBall;
	avgScorePerSecond = param.avgScorePerSecond;
	totalDuration = param.totalDuration;
	totalScore = param.totalScore;
}

void GameData::ReadData(string fileName)
{
	string line[MAX_LINE_BUFFER];
	ifstream dataFile;

	dataFile.open(fileName);

	int currentLine = 0;

	if (dataFile.is_open())
	{
		if (currentLine < MAX_LINE_BUFFER)
		{
			while (!dataFile.eof())
			{
				getline(dataFile, line[currentLine]);
				currentLine++;
			}
		}
		else
			cout << "File too big!" << endl;
	}
	else
	{
		cout << "Unable to open file!" << endl;
		return;
	}

	numberOfBalls = stoi(line[1]);
	balls = new BallData[numberOfBalls];

	for (int i = 0; i < numberOfBalls; i++)
	{
		balls[i].m_duration = stoi(line[(i + 1) * 2]);
		balls[i].m_score = stoi(line[((i + 1) * 2) + 1]);
	}

	int lineOffset = (numberOfBalls * 2) + 2;

	totalDuration = stoi(line[lineOffset]);
	totalScore = stoi(line[lineOffset + 1]);
	avgScorePerSecond = stoi(line[lineOffset + 2]);
	avgScorePerBall = stoi(line[lineOffset + 3]);
	avgDurationPerBall = stoi(line[lineOffset + 4]);
}

GameData GameData::Difference(GameData comparee)
{
	GameData diff;

	diff.avgDurationPerBall = comparee.avgDurationPerBall - avgDurationPerBall;
	diff.avgScorePerBall = comparee.avgScorePerBall - avgScorePerBall;
	diff.avgScorePerSecond = comparee.avgScorePerSecond - avgScorePerSecond;
	diff.totalDuration = comparee.totalDuration - totalDuration;
	diff.totalScore = comparee.totalScore - totalScore;

	if (numberOfBalls != comparee.numberOfBalls)
		cout << "Unable to compare individual ball data. Different number of balls in each set of data." << endl;
	else
	{
		diff.numberOfBalls = numberOfBalls;
		diff.balls = new BallData[diff.numberOfBalls];
		for (int i = 0; i < numberOfBalls; i++)
		{
			diff.balls[i].m_duration = comparee.balls[i].m_duration - balls[i].m_duration;
			diff.balls[i].m_score = comparee.balls[i].m_score - balls[i].m_score;
		}
	}

	return diff;
}

void GameData::OutputData()
{
	cout << "Total Balls Played:\t\t" << numberOfBalls << endl;
	cout << "Total Time Ball In Play:\t" << totalDuration << "s" << endl;
	cout << "Total Score:\t\t\t" << totalScore << endl;
	cout << "Average Score Per Second:\t" << avgScorePerSecond << endl;
	cout << "Average Score Per Ball:\t\t" << avgScorePerBall << endl;
	cout << "Average Duration Per Ball:\t" << avgDurationPerBall << "s" << endl << endl;

	cout << "Data For Individual Balls:" << endl;
	for (int i = 0; i < numberOfBalls; i++)
	{
		cout << "Ball" << i << ": ";
		cout << "\tDuration: " << balls[i].m_duration << "s" << endl;
		cout << "\tScore:\t  " << balls[i].m_score << endl << endl;
	}
}