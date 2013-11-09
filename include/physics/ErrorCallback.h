#ifndef ERRORCALLBACK_H
#define ERRORCALLBACK_H

#include "PxPhysicsAPI.h"

namespace Physics
{
	using namespace physx;

	class ErrorCallback :
		public PxDefaultErrorCallback
	{
	public:
		ErrorCallback(void);
		~ErrorCallback(void);

		virtual void reportError(PxErrorCode::Enum code, const char* message, const char* file, int line) {};
	};
}

#endif // ERRORCALLBACK_H