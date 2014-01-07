/*-------------------------------------------------------------------------\
| File: PHYSICS.H															|
| Desc: Declarations for a namespace used to simulate physics with			|
|		NVIDIA PhysX.														|
| Definition File: PHYSICS.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PHYSICS_H
#define PHYSICS_H

#include "globals.h"
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
	PxCooking* PxGetCooking();

	// -- Utility Functions --
	PxMaterial* cpyMaterial(PxMaterial* src);
	PxShape* cpyShape(PxShape* src);
	void AddDistanceJoint(PxRigidActor* actor0, PxTransform& localFrame0, PxRigidActor* actor1, PxTransform& localFrame1,
		PxDistanceJointFlag::Enum flags = PxDistanceJointFlag::Enum::eSPRING_ENABLED, PxReal stiffness = 1.f, PxReal damping = 1.f);

	// -- Convex Mesh Functions
	PxConvexMesh* CreateConvexMesh(Vec3* const verts, const int& nVerts, const Vec3& scale);
	PxConvexMesh* Cook(const PxConvexMeshDesc& desc);

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

	/* Base class for all event callbacks */
	class SimulationEventCallback : public PxSimulationEventCallback
	{
	protected:
		bool m_isTriggered;
	public:
		SimulationEventCallback();
		virtual ~SimulationEventCallback();

		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);

		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {}
		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}

		bool IsTriggered();
	};

	class Scene : private Uncopyable
	{
	protected:
			PxScene* m_scene;
			SimulationEventCallback* m_eventCallback;
			bool m_pause;

		public:
			Scene();
			~Scene();

			void Init();
			bool IsPaused() const;
			void TogglePause();

			void UpdatePhys(Fl32 deltaTime);

			void SetEventCallback(SimulationEventCallback* eventCallback);
			SimulationEventCallback* GetSimulationEventCallback();

			std::vector<PxRigidActor*> GetActors(PxActorTypeSelectionFlags flags, bool rendering = false) const;

			void Add(Actor* actor);
	};
}

#endif // PHYSICS_H