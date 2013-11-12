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

		virtual void Create();
	};

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

		virtual void Create();
	};

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