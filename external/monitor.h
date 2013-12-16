#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "log.h"
#include <vector>
#include <string>

#define MONITOR_LOG "monitorLog.txt"

struct BallData
{
	BallData();
	BallData(int duration, int score);
	int m_duration;
	int m_score;
};

struct MonitorData
{
	MonitorData();

	/* Stores data for individual balls */
	std::vector<BallData> m_balls;

	int totalDuration();
	int totalScore();
	int avgScoresPerBall(); // sigma(scoresPerBall)/scoresPerBall.length
	int avgDurationPerBall(); // sigma(secondsPerBall)/secondsPerBall.length
	int avgScorePerSecond(); // gameScore/gameDuration
};

class Monitor
{
private:
	MonitorData data;
public:
	Monitor();
	~Monitor();
	
	/* Add data for a ball */
	void AddBall(int score, int durationSeconds);

	void OutputData();
};

#endif // _MONITOR_H_