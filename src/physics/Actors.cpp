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
	ShapeActor::ShapeActor(Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType) : Actor(pose, density, aType)
	{
		m_shape = NULL;
		m_material = cpyMaterial(material);
		m_color = color;
	}

	ShapeActor::ShapeActor(const ShapeActor& param) :
		Actor(param.m_pose, param.m_density, param.m_aType)
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
	Box::Box(Transform pose, Vec3 dimensions, Fl32 density, const Vec3& color, PxMaterial* material,
		ActorType aType)
		: ShapeActor(pose, density, material, color, aType)
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
		void* shape;

		if(m_aType == DynamicActor)
		{
			shape = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
			static_cast<PxRigidDynamic*>(shape)->createShape(PxBoxGeometry(m_dimensions), *m_material, IDENTITY_TRANS);
			PxRigidBodyExt::setMassAndUpdateInertia(*static_cast<PxRigidDynamic*>(shape), m_density);
			m_actor = static_cast<PxRigidDynamic*>(shape);
		}
		else
		{
			shape = Physics::PxGetPhysics()->createRigidStatic(m_pose);
			static_cast<PxRigidStatic*>(shape)->createShape(PxBoxGeometry(m_dimensions), *m_material, IDENTITY_TRANS);
			m_actor = static_cast<PxRigidStatic*>(shape);
		}
		if(m_actor)
			m_actor->userData = &m_color;
		else
			Log::Write("Exc: Error creating Box Actor. m_actor is NULL!", ENGINE_LOG);
	}

	Box::~Box()
	{

	}
	/*-------------------------------------------------------------------------\
	|							SPHERE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Sphere::Sphere(Transform pose, Fl32 radius, Fl32 density, const Vec3& color, PxMaterial* material,
		ActorType aType)
		: ShapeActor(pose, density, material, color, aType)
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
		void* shape;

		if(m_aType == DynamicActor)
		{
			shape = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
			static_cast<PxRigidDynamic*>(shape)->createShape(PxSphereGeometry(m_radius), *m_material, IDENTITY_TRANS);
			PxRigidBodyExt::setMassAndUpdateInertia(*static_cast<PxRigidDynamic*>(shape), m_density);
			m_actor = static_cast<PxRigidDynamic*>(shape);
		}
		else
		{
			shape = Physics::PxGetPhysics()->createRigidStatic(m_pose);
			static_cast<PxRigidStatic*>(shape)->createShape(PxSphereGeometry(m_radius), *m_material, IDENTITY_TRANS);
			m_actor = static_cast<PxRigidStatic*>(shape);
		}
		if(m_actor)
			m_actor->userData = &m_color;
		else
			Log::Write("Exc: Error creating Sphere Actor. m_actor is NULL!", ENGINE_LOG);
	}

	Sphere::~Sphere()
	{

	}
	/*-------------------------------------------------------------------------\
	|							PLANE DEFINITIONS								 |
	\-------------------------------------------------------------------------*/
	Plane::Plane(Vec3 normal, Fl32 distance, const Vec3& color, PxMaterial* material)
		: ShapeActor(IDENTITY_TRANS, 0.f, material, color, StaticActor)
	{
		m_normal = normal;
		m_distance = distance;
	}

	Plane::Plane(const Plane& param) : ShapeActor(param)
	{
		m_normal = param.m_normal;
		m_distance = param.m_distance;
	}

	Plane& Plane::operator=(const Plane& param)
	{
		if(this == &param)
			return *this;
		else
		{
			ShapeActor::operator=(param);
			m_normal = param.m_normal;
			m_distance = param.m_distance;
			return *this;
		}
	}

	Plane::~Plane()
	{

	}

	void Plane::Create()
	{
		PxRigidStatic* plane = PxCreatePlane(*PHYSICS, PxPlane(m_normal, m_distance), *DEFAULT_MATERIAL);
		m_actor = plane;
		m_actor->userData = &m_color; //pass color parameter to renderer
	}
}