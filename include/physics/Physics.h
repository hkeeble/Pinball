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
#include "log.h" // Log for logging program

#define DEFAULT_DENSITY		0.f
#define DEFAULT_COLOR		Vec3(.9f, 0.f, 0.f)
#define DEFAULT_MATERIAL	Physics::PxGetPhysics()->createMaterial(0.0f, 0.0f, 0.0f)
#define IDENTITY			Transform(PxIdentity)

typedef physx::PxVec3		Vec3;
typedef physx::PxVec2		Vec2;
typedef physx::PxTransform	Transform;

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

			void Update(PxReal deltaTime);

			std::vector<PxRigidActor*> GetActors(PxActorTypeSelectionFlags flags) const;

			void Add(Actor* actor);
	};

	class Actor
	{
		protected:
			Actor();
			Actor(Transform pose, PxReal density);
			Actor(const Actor& param);
			virtual Actor& operator=(const Actor& param);

			PxActor* m_actor;
			Transform m_pose;
			PxReal m_density;

		public:
			virtual ~Actor();

			PxActor* Get();

			virtual void Create() = 0;
	};

	/* Represents an actor that contains a PxShape (used for virtual resource management) */
	class ShapeActor : public Actor
	{
	protected:
		ShapeActor(Transform pose = IDENTITY, PxReal density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
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