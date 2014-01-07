/*-------------------------------------------------------------------------\
| File: LOG.CPP																|
| Desc: Provides defintions for functions used to log information at		|
|		runtime.															|
| Declaration File: LOG.H													|												
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "log.h"

char* Log::m_workingDir = NULL;

void Log::Write(const char* text, const char* fileName)
{
	std::string temp(m_workingDir);
	std::string temp2(fileName);
	temp = temp + temp2;

	std::ofstream file;
	file.open (temp, std::ios::app|std::ios::out);
	file << text;
	file.close();

	Out(text);
}

void Log::SetWorkingDir(char* newWorkingDir)
{
	m_workingDir = (char*)calloc(strlen((char*)newWorkingDir), sizeof(WCHAR));
	strcpy((char*)m_workingDir, (char*)newWorkingDir);
}

char* Log::GetWorkingDir()
{
	return m_workingDir;
}

char* GetCurrentDir()
{
	// Get Path
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	
	// Convert to string
	std::stringstream ss;
	ss << path;
	std::string s = ss.str();

	// Remove Executable Name
	unsigned int pos = s.find_last_of("\\");
	s = s.substr(0, pos+1);

	char* realPath = (char*)calloc(strlen(s.c_str()), sizeof(char));
	strcpy(realPath, s.c_str());

	return realPath;
}

void Log::Shutdown()
{
	Log::Write("\n", ENGINE_LOG);
}

void InitLog(const char* fileName)
{
	Log::SetWorkingDir(GetCurrentDir());

	time_t* t = new time_t();
	struct tm* now = new tm();

	*t = time(0);
	localtime_s(now, t);

	std::string s;
	s += " ----- Instance Run on " + std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_mday) + "/" + std::to_string(now->tm_year + 1900) + " at " +
		std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + " ----- \n";

	Log::Write(s.c_str(), fileName);

	if (t)
		delete t;
	if (now)
		delete now;
}