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

			std::vector<PxRigidActor*> GetActors(PxActorTypeSelectionFlags flags) const;

			void Add(Actor* actor);
	};

	class Actor
	{
		protected:
			Actor();
			Actor(Transform pose, Fl32 density);
			Actor(const Actor& param);
			virtual Actor& operator=(const Actor& param);

			PxActor* m_actor;
			Transform m_pose;
			Fl32 m_density;

		public:
			virtual ~Actor();

			PxActor* Get();

			virtual void Create() = 0;

			#ifdef _DEBUG
			void PrintPose() const;
			#endif
	};

	/* Represents an actor that contains a PxShape (used for virtual resource management) */
	class ShapeActor : public Actor
	{
	protected:
		ShapeActor(Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR);
		ShapeActor(const ShapeActor& param);
		virtual ShapeActor& operator=(const ShapeActor& param);

		virtual ~ShapeActor();

		PxShape* m_shape;
		PxMaterial* m_material;
		Vec3 m_color;
	public:
		virtual void Create() = 0;
	};
}

#endif // PHYSICS_H