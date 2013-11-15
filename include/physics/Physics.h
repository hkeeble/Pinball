/*-------------------------------------------------------------------------\
| File: PHYSICS.H															|
| Desc: Declarations for a namespace used to simulate physics with			|
|		NVIDIA PhysX.														|
| Definition File: PHYSICS.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PHYSICS_H
#define PHYSICS_H

#include "../globals.h"
#include "ErrorCallback.h" // Custom Error Callback Class
#include "uncopyable.h" // uncopyable base class
#include <vector> // include vector for actor storage
#include "log.h" // Log for logging program

namespace Physics
{
	using namespace physx;

	// -- Class Declarations --
	class Scene;
	class Actor;

	// -- PhysX Functions --
	void PxInit();
	void PxRelease();

	// -- Accessor Functions --
	PxPhysics* PxGetPhysics();
	PxMaterial* PxGetDefaultMaterial();

	// -- Utility Functions --
	PxMaterial* cpyMaterial(PxMaterial* src);
	PxShape* cpyShape(PxShape* src);

	enum ActorType
	{
		DynamicActor,
		StaticActor
	};

	enum GeometryType
	{
		BoxGeometry,
		SphereGeometry
	};

	class Scene : private Uncopyable
	{
	protected:
			PxScene* m_scene;
			bool m_pause;

		public:
			Scene();
			~Scene();

			void Init();
			bool IsPaused() const;
			void TogglePause();

			void Update(Fl32 deltaTime);

			std::vector<PxRigidActor*> GetActors(PxActorTypeSelectionFlags flags, bool rendering = false) const;

			void Add(Actor* actor);
	};
}

#endif // PHYSICS_H