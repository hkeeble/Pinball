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
	
	data.m_balls.push_back(BallData(durationSeconds, score));
}

void Monitor::OutputData()
{
	char* currentFilePath = new char[strlen(GetCurrentDir()) + strlen(MONITOR_LOG)];
	strcpy(currentFilePath, GetCurrentDir());
	strcat(currentFilePath, MONITOR_LOG);
	remove(currentFilePath);

	InitLog(MONITOR_LOG);

	std::string n = std::to_string(data.m_balls.size()) + "\n";
	Log::Write(n.c_str(), MONITOR_LOG);

	// Data for each ball
	for (int i = 0; i < data.m_balls.size(); i++)
	{
		std::string s = std::to_string(data.m_balls.at(i).m_duration) + "\n" + std::to_string(data.m_balls.at(i).m_score) + "\n";
		Log::Write(s.c_str(), MONITOR_LOG);
	}

	// Overall data
	std::string s2 =	std::to_string(data.totalDuration()) + "\n" +
						std::to_string(data.totalScore()) + "\n" +
						std::to_string(data.avgScorePerSecond()) + "\n" +
						std::to_string(data.avgScoresPerBall()) + "\n" +
						std::to_string(data.avgDurationPerBall()) + "\n";

	Log::Write(s2.c_str(), MONITOR_LOG);
}