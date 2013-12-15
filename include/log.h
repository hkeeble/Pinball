/*-------------------------------------------------------------------------\
| File: LOG.H																|
| Desc: Provides declarations for functions used to log information at		|
|		runtime.															|
| Definition File: LOG.CPP													|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <ctime>
#include "globals.h"

#define Out printf

// Log Files
#define ENGINE_LOG "logMain.txt"

char* GetCurrentDir();

extern void InitLog(const char* fileName);

class Log
{
private:
	Log();
	~Log();
	Log(const Log& param);
	Log& operator=(const Log& param);

	static char* m_workingDir;
	static char* m_curFileName;
public:
	static void Write(const char* text, const char* fileName);

	static void SetWorkingDir(char* newWorkingDir);
	static char* GetWorkingDir();

	static void Shutdown();
};

#endif // _LOG_H_