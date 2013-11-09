/*-------------------------------------------------------------------------\
| File: PHYSICS.H															|
| Desc: Declarations for a namespace used to simulate physics with			|
|		NVIDIA PhysX.														|
| Definition File: PHYSICS.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PHYSICS_H
#define PHYSICS_H

#include "PxPhysicsAPI.h" // Include PhysX
#include "ErrorCallback.h" // Custom Error Callback Class
#include "uncopyable.h" // uncopyable base class
#include <vector> // include vector for actor storage

namespace Physics
{
	using namespace physx;

	// -- PhysX Functions --
	void PxInit();
	void PxRelease();

	// -- Accessor Functions --
	PxPhysics* PxGetPhysics();
	PxMaterial* PxGetDefaultMaterial();

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

			void Update(PxReal deltaTime);

			std::vector<PxRigidActor*> GetActors() const;
	};

	class Actor
	{
		protected:
			PxActor* m_actor;
			PxTransform m_pose;
			PxReal m_density;

		public:
			Actor();
			~Actor();

			PxActor* Get();
	};

}

#endif // PHYSICS_H