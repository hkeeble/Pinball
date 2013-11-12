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

			std::vector<PxRigidActor*> GetActors(PxActorTypeSelectionFlags flags) const;

			void Add(Actor* actor);
	};

	class Actor
	{
		protected:
			Actor();
			Actor(Transform pose, Fl32 density, ActorType m_aType);
			Actor(const Actor& param);
			virtual Actor& operator=(const Actor& param);

			PxActor* m_actor;
			ActorType m_aType;
			Transform m_pose;
			Fl32 m_density;

		public:
			virtual ~Actor();

			PxActor* Get();

			virtual void Create() = 0;

			// Functions Used for Debugging
			#ifdef _DEBUG
			void PrintPose() const;
			#endif
	};

	/* Represents an actor that contains a PxShape (used for virtual resource management) */
	class ShapeActor : public Actor
	{
	protected:
		ShapeActor(Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
		ShapeActor(const ShapeActor& param);
		virtual ShapeActor& operator=(const ShapeActor& param);

		virtual ~ShapeActor();

		PxGeometryHolder m_geometry;
		PxMaterial* m_material;
		Vec3 m_color;

	public:
		virtual void Create();
	};

	class CompoundShapeActor : public Actor
	{
	protected:
		CompoundShapeActor(Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
		CompoundShapeActor(const CompoundShapeActor& param);
		virtual ShapeActor& operator=(const CompoundShapeActor& param);

		virtual ~CompoundShapeActor();

		PxShape* m_shape;
		PxMaterial* m_material;
		Vec3 m_color;
	public:
		virtual void Create() = 0;
	};
}

#endif // PHYSICS_H