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
	/* Union to represent actor */
	union ActorUnion
	{
		PxRigidStatic* staticActor;
		PxRigidDynamic* dynamicActor;
	};

	/* Abstract Actor Class */
	class Actor
	{
	protected:
		ActorUnion m_actor;
		ActorType m_aType;
		Fl32 m_density;
		Transform m_pose;

	public:
		Actor();
		Actor(Transform pose, Fl32 density, ActorType m_aType);
		Actor(const Actor& param);
		virtual Actor& operator=(const Actor& param);
		virtual ~Actor();

		Transform Pose();

		virtual void Create() = 0;

		virtual ActorUnion Get();

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
	|						COMPOUND SHAPE ACTOR								|
	\-------------------------------------------------------------------------*/
	/* Use as a base to derive complex actors consisting of more than one shape */
	class CompoundShapeActor : public Actor
	{
	protected:
		PxGeometryHolder* m_geometrys;
		PxShape* m_shapes;
		PxMaterial* m_material;
		Vec3 m_color;
		int nShapes;

	public:
		CompoundShapeActor(int numberOfShapes = 0, Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, PxMaterial* material = DEFAULT_MATERIAL,
			Vec3 color = DEFAULT_COLOR, ActorType aType = DEFAULT_ACTOR_TYPE);
		CompoundShapeActor(const CompoundShapeActor& param);
		virtual CompoundShapeActor& operator=(const CompoundShapeActor& param);

		virtual ~CompoundShapeActor();

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
	|								WEDGE										|
	\-------------------------------------------------------------------------*/
	class Wedge : public ShapeActor
	{
	private:
		Vec3 m_scale;
	public:
		Wedge(Transform pose = IDENTITY_TRANS, Fl32 density = DEFAULT_DENSITY, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL, Vec3 scale = Vec3(1, 1, 1), ActorType aType = DEFAULT_ACTOR_TYPE);
		Wedge(const Wedge& param);
		virtual Wedge& operator=(const Wedge& param);
		virtual ~Wedge();

		virtual void Create() override;
	};
}

#endif _ACTORS_H_