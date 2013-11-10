/*-------------------------------------------------------------------------\
| File: ACTORS.CPP															|
| Desc: Definitions of actor types											|
| Definition File: ACTORS.H													|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "Actors.h"

namespace Physics
{
	/*------------------------------------------------------------------------\
	|							SHAPEACTOR DEFINITIONS							 |
	\-------------------------------------------------------------------------*/
	ShapeActor::ShapeActor(Transform pose, PxReal density, PxMaterial* material, Vec3 color) : Actor(pose, density)
	{
		cpyMaterial(m_material, material);
		m_color = color;
	}

	ShapeActor::ShapeActor(const ShapeActor& param) :
		Actor(param.m_pose, param.m_density)
	{
		cpyShape(m_shape, param.m_shape);
		cpyMaterial(m_material, param.m_material);
	}

	ShapeActor& ShapeActor::operator=(const ShapeActor& param)
	{
		if(this == &param)
			return *this;
		else
		{
			if(m_shape)
				m_shape->release();
			cpyShape(m_shape, param.m_shape);

			if(m_material)
				m_material->release();
			cpyMaterial(m_material, param.m_material);

			if(m_shape)
			{
				if(m_material)
					m_shape->setMaterials((PxMaterial* const*)m_material, 1);
				else
					m_shape->setMaterials((PxMaterial* const*)DEFAULT_MATERIAL, 1);
			}

			return *this;
		}
	}

	ShapeActor::~ShapeActor()
	{
		if(m_shape)
			m_shape->release();
		if(m_material)
			m_material->release();
	}

	/*------------------------------------------------------------------------\
	|							BOX DEFINITIONS									 |
	\-------------------------------------------------------------------------*/
	Box::Box(PxTransform pose, Vec3 dimensions, PxReal density, const Vec3& color, PxMaterial* material)
		: ShapeActor(pose, density, material, color)
	{
		m_dimensions = dimensions;
	}

	Box::Box(const Box& param) : ShapeActor(param)
	{
		m_dimensions = param.m_dimensions; 
	}

	Box& Box::operator=(const Box& param)
	{
		if(this == &param)
			return *this;
		else
		{
			ShapeActor::operator=(param);
			m_dimensions = param.m_dimensions;
			return *this;
		}
	}

	void Box::Create()
	{
		PxRigidDynamic* box = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
		box->createShape(PxBoxGeometry(m_dimensions), *m_material, IDENTITY);
		PxRigidBodyExt::setMassAndUpdateInertia(*box, m_density);
		m_actor = box;
		m_actor->userData = &m_color;
	}

	Box::~Box()
	{
		ShapeActor::~ShapeActor();
	}
	/*-------------------------------------------------------------------------\
	|							SPHERE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Sphere::Sphere(PxTransform pose, PxReal radius, PxReal density, const Vec3& color, PxMaterial* material)
		: ShapeActor(pose, density, material, color)
	{
		m_radius = radius;
	}

	Sphere::Sphere(const Sphere& param) : ShapeActor(param)
	{
		m_radius = param.m_radius; 
	}

	Sphere& Sphere::operator=(const Sphere& param)
	{
		if(this == &param)
			return *this;
		else
		{
			ShapeActor::operator=(param);
			m_radius = param.m_radius;
			return *this;
		}
	}

	void Sphere::Create()
	{
		PxRigidDynamic* sphere = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
		sphere->createShape(PxSphereGeometry(m_radius), *m_material, IDENTITY);
		PxRigidBodyExt::setMassAndUpdateInertia(*sphere, m_density);
		m_actor = sphere;
		m_actor->userData = &m_color;
	}

	Sphere::~Sphere()
	{
		ShapeActor::~ShapeActor();
	}
}