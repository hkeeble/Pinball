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
	char* temp = (char*)calloc(strlen(m_workingDir), sizeof(char));

	strcpy(temp, m_workingDir);
	strcat(temp, fileName);

	std::ofstream file;
	file.open (temp, std::ios::app);
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