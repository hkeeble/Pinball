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
	protected:
		virtual void Create();
	public:
		Box(PxTransform pose = IDENTITY, Vec3 dimensions = Vec3(.5f, .5f, .5f),
			PxReal density = DEFAULT_DENSITY, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL);
		Box(const Box& param);
		virtual Box& operator=(const Box& param);
		virtual ~Box();
	};

	class Sphere : public ShapeActor
	{
	private:
		PxReal m_radius;
	protected:
		virtual void Create();
	public:
		Sphere(PxTransform pose = IDENTITY, PxReal radius = 1.f,
			PxReal density = DEFAULT_DENSITY, const Vec3& color = DEFAULT_COLOR,
			PxMaterial* material = DEFAULT_MATERIAL);
		Sphere(const Sphere& param);
		virtual Sphere& operator=(const Sphere& param);
		virtual ~Sphere();
	};
}

#endif _ACTORS_H_