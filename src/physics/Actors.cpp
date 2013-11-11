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
	ShapeActor::ShapeActor(Transform pose, Fl32 density, PxMaterial* material, Vec3 color) : Actor(pose, density)
	{
		m_shape = NULL;
		m_material = cpyMaterial(material);
		m_color = color;
	}

	ShapeActor::ShapeActor(const ShapeActor& param) :
		Actor(param.m_pose, param.m_density)
	{
		m_shape = cpyShape(param.m_shape);
		m_material = Physics::PxGetPhysics()->createMaterial(param.m_material->getStaticFriction(),
						param.m_material->getDynamicFriction(), param.m_material->getRestitution());
		m_color = param.m_color;
	}

	ShapeActor& ShapeActor::operator=(const ShapeActor& param)
	{
		if(this == &param)
			return *this;
		else
		{
			if(m_shape)
				m_shape->release();
			m_shape = cpyShape(param.m_shape);

			if(m_material)
				m_material->release();
			m_material = cpyMaterial(param.m_material);

			if(m_shape)
			{
				if(m_material)
					m_shape->setMaterials((PxMaterial* const*)m_material, 1);
				else
					m_shape->setMaterials((PxMaterial* const*)DEFAULT_MATERIAL, 1);
			}

			m_color = param.m_color;

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
	Box::Box(Transform pose, Vec3 dimensions, Fl32 density, const Vec3& color, PxMaterial* material)
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
		box->createShape(PxBoxGeometry(m_dimensions), *m_material, IDENTITY_TRANS);
		PxRigidBodyExt::setMassAndUpdateInertia(*box, m_density);
		m_actor = box;
		m_actor->userData = &m_color;
	}

	Box::~Box()
	{

	}
	/*-------------------------------------------------------------------------\
	|							SPHERE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Sphere::Sphere(Transform pose, Fl32 radius, Fl32 density, const Vec3& color, PxMaterial* material)
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
		sphere->createShape(PxSphereGeometry(m_radius), *m_material, IDENTITY_TRANS);
		PxRigidBodyExt::setMassAndUpdateInertia(*sphere, m_density);
		m_actor = sphere;
		m_actor->userData = &m_color;
	}

	Sphere::~Sphere()
	{

	}
}