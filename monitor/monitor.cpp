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
	/* Calculate number of files already in directory */
	WIN32_FIND_DATA fData;
	char* dir = GetCurrentDir();
	std::string d = dir;
	d.append("/analyzer/*.txt");

	HANDLE find = FindFirstFile(d.c_str(), &fData);
	int fCount = 0;
	if (find != INVALID_HANDLE_VALUE)
	{
		fCount++;
		while (FindNextFile(find, &fData) == TRUE)
			fCount++;
	}

	std::string logPath = "analyzer/" MONITOR_LOG + std::to_string(fCount) + MONITOR_LOG_EXT;

	InitLog(logPath.c_str());

	std::string n = std::to_string(data.m_balls.size()) + "\n";
	Log::Write(n.c_str(), logPath.c_str());

	// Data for each ball
	for (int i = 0; i < data.m_balls.size(); i++)
	{
		std::string s = std::to_string(data.m_balls.at(i).m_duration) + "\n" + std::to_string(data.m_balls.at(i).m_score) + "\n";
		Log::Write(s.c_str(), logPath.c_str());
	}

	// Overall data
	std::string s2 =	std::to_string(data.totalDuration()) + "\n" +
						std::to_string(data.totalScore()) + "\n" +
						std::to_string(data.avgScorePerSecond()) + "\n" +
						std::to_string(data.avgScoresPerBall()) + "\n" +
						std::to_string(data.avgDurationPerBall()) + "\n";

	Log::Write(s2.c_str(), logPath.c_str());
}