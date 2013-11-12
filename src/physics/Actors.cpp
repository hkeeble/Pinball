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
		m_material = cpyMaterial(material);
		m_color = color;
		m_geometry = PxGeometryHolder();
	}

	ShapeActor::ShapeActor(const ShapeActor& param) :
		Actor(param.m_pose, param.m_density, param.m_aType)
	{
		m_material = cpyMaterial(param.m_material);
		m_color = param.m_color;
		m_geometry = param.m_geometry;

		// Create Shape
		Create();
	}

	ShapeActor& ShapeActor::operator=(const ShapeActor& param)
	{
		if(this == &param)
			return *this;
		else
		{
			if(m_material)
				m_material->release();
			m_material = cpyMaterial(param.m_material);
			m_geometry = param.m_geometry;
			m_color = param.m_color;

			// Create Shape
			Create();

			return *this;
		}
	}

	ShapeActor::~ShapeActor()
	{
		if(m_material)
			m_material->release();
	}

	void ShapeActor::Create()
	{
		void* shape = NULL;

		if(m_aType == DynamicActor)
		{
			PxRigidDynamic* ptr = StaticCast(shape, PxRigidDynamic*); // Receive Correctly Cast Pointer
			ptr = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
			ptr->createShape(m_geometry.any(), *m_material, IDENTITY_TRANS);
			PxRigidBodyExt::setMassAndUpdateInertia(*ptr, m_density);
			m_actor = ptr;
		}
		else
		{
			PxRigidStatic* ptr = StaticCast(shape, PxRigidStatic*); // Receive Correctly Cast Pointer
			ptr = Physics::PxGetPhysics()->createRigidStatic(m_pose);
			ptr->createShape(m_geometry.any(), *m_material, IDENTITY_TRANS);
			m_actor = ptr;
		}
		if(m_actor)
			m_actor->userData = &m_color;
		else
			Log::Write("Exc: Error creating Box Actor. m_actor is NULL!", ENGINE_LOG);
	}
	/*------------------------------------------------------------------------\
	|							BOX DEFINITIONS									 |
	\-------------------------------------------------------------------------*/
	Box::Box(Transform pose, Vec3 dimensions, Fl32 density, const Vec3& color, PxMaterial* material,
		ActorType aType)
		: ShapeActor(pose, density, material, color, aType)
	{
		m_dimensions = dimensions;
		m_geometry.storeAny(PxBoxGeometry(m_dimensions));
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
		m_geometry.storeAny(PxSphereGeometry(m_radius));
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