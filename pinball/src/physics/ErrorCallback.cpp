#include "ErrorCallback.h"
#include "log.h"

namespace Physics
{
	ErrorCallback::ErrorCallback(void)
	{

	}


	ErrorCallback::~ErrorCallback(void)
	{

	}

	void ErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		std::string string = "\n--- PHYSX ERROR ---\nPX ERROR CODE: " + std::to_string((int)code) + "\nMsg: " + message + "\n\n";
		Log::Write(string.c_str(), ENGINE_LOG);
	}
}