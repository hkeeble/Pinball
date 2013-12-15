#include "monitor.h"

BallData::BallData()
{
	m_duration = 0;
	m_score = 0;
}

BallData::BallData(int duration, int score)
{
	m_duration = duration;
	m_score = score;
}

MonitorData::MonitorData()
{
	int gameDuration = 0;
	int gameScore = 0;
	m_balls = std::vector<BallData>();
}

int MonitorData::totalDuration()
{
	int dur = 0;
	for (std::vector<BallData>::iterator iter = m_balls.begin(); iter != m_balls.end(); iter++)
		dur += iter->m_duration;
	return dur;
}


int MonitorData::totalScore()
{
	int score = 0;
	for (std::vector<BallData>::iterator iter = m_balls.begin(); iter != m_balls.end(); iter++)
		score += iter->m_score;
	return score;
}

int MonitorData::avgScoresPerBall() // sigma(scoresPerBall)/scoresPerBall.length
{
	int total = totalScore();
	return total / m_balls.size();
}

int MonitorData::avgDurationPerBall() // sigma(secondsPerBall)/secondsPerBall.length
{
	int total = totalDuration();
	return total / m_balls.size();
}

int MonitorData::avgScorePerSecond() // gameScore/gameDuration
{
	return totalScore() / totalDuration();
}

Monitor::Monitor()
{
	data = MonitorData();
}

Monitor::~Monitor()
{

}

void Monitor::AddBall(int score, int durationSeconds)
{
	if (data.m_balls.size() > 0)
	{
		score -= data.totalScore();
		durationSeconds -= data.totalDuration();
	}
	
	data.m_balls.push_back(BallData(score, durationSeconds));
}

void Monitor::OutputData()
{
	InitLog(MONITOR_LOG);

	for (int i = 0; i < data.m_balls.size(); i++)
	{
		std::string s = "\nBall" + std::to_string(i) + "\n";
		Log::Write(s.c_str(), MONITOR_LOG);
	}
}