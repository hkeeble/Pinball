/*-------------------------------------------------------------------------\
| File: ACTORS.H															|
| Desc: Declarations of actor types											|
| Definition File: ACTORS.CPP												|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef _ACTORS_H_
#define _ACTORS_H_

#include "Physics.h"
#include "log.h"

namespace Physics
{
	/*-------------------------------------------------------------------------\
	|								ACTOR										|
	\-------------------------------------------------------------------------*/
	/* Abstract Actor Class */
	class Actor
	{
		protected:
			Actor();
			Actor(Transform pose, Fl32 density, ActorType m_aType);
			Actor(const Actor& param);
			virtual Actor& operator=(const Actor& param);

			PxRigidActor* m_actor;
			ActorType m_aType;
			Fl32 m_density;
			Transform m_pose;

		public:
			virtual ~Actor();

			PxRigidActor* Get();

			virtual void Create() = 0;

			// Functions Used for Debugging
			#ifdef _DEBUG
			void PrintPose() const;
			#endif
	};

	/*-------------------------------------------------------------------------\
	|								SHAPE ACTOR									|
	\-------------------------------------------------------------------------*/
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
		PxShape* m_shape;
		PxMaterial* m_material;
		Vec3 m_color;

	public:
		virtual void Create();

		PxShape* GetShape();
	};

	/*-------------------------------------------------------------------------\
	|								CONVEX MESH ACTOR							|
	\-------------------------------------------------------------------------*/
	/* Represents an actor with a convex mesh */
	class ConvexMeshActor : public Actor
	{

	};

	/*-------------------------------------------------------------------------\
	|						COMPOUND SHAPE ACTOR								|
	\-------------------------------------------------------------------------*/
	/* Use as a base to derive complex actors consisting of more than one shape */
	class CompoundShapeActor : public Actor, Uncopyable
	{
	protected:
		CompoundShapeActor(int numberOfShapes = 1, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
		CompoundShapeActor(const CompoundShapeActor& param);
		virtual CompoundShapeActor& operator=(const CompoundShapeActor& param);

		virtual ~CompoundShapeActor();

		PxGeometryHolder* m_geometrys;
		PxShape* m_shapes;
		PxMaterial* m_material;
		Vec3 m_color;

		int nShapes;

		PxShape* GetShapes();
	public:
		virtual void Create() = 0;
	};

	/*-------------------------------------------------------------------------\
	|								BOX											|
	\-------------------------------------------------------------------------*/
	class Box : public ShapeActor
	{
	private:
		Vec3 m_dimensions;
	public:
		Box(Transform pose = IDENTITY_TRANS, Vec3 dimensions = Vec3(.5f, .5f, .5f),
			Fl32 density = DEFAULT_DENSITY, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL, ActorType aType = DEFAULT_ACTOR_TYPE);
		Box(const Box& param);
		virtual Box& operator=(const Box& param);
		virtual ~Box();

		Vec3 Dimensions() const;
	};

	/*-------------------------------------------------------------------------\
	|								SPHERE										|
	\-------------------------------------------------------------------------*/
	class Sphere : public ShapeActor
	{
	private:
		Fl32 m_radius;
	public:
		Sphere(Transform pose = IDENTITY_TRANS, Fl32 radius = 1.f,
			Fl32 density = DEFAULT_DENSITY, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL, ActorType aType = DEFAULT_ACTOR_TYPE);
		Sphere(const Sphere& param);
		virtual Sphere& operator=(const Sphere& param);
		virtual ~Sphere();

		Fl32 Radius() const;
	};

	/*-------------------------------------------------------------------------\
	|								PLANE										|
	\-------------------------------------------------------------------------*/
	class Plane : public ShapeActor
	{
	private:
		Vec3 m_normal;
		Fl32 m_distance;
	public:
		Plane(Vec3 normal = UP_VECTOR, Fl32 distance = 0.f, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL);
		Plane(const Plane& param);
		virtual Plane& operator=(const Plane& param);
		virtual ~Plane();

		virtual void Create();
	};
}

#endif _ACTORS_H_