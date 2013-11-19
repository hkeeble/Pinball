/*-------------------------------------------------------------------------\
| File: ACTORS.CPP															|
| Desc: Definitions of actor types											|
| Definition File: ACTORS.H													|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "Actors.h"

namespace Physics
{
	/*-------------------------------------------------------------------------\
	|							ACTOR DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Actor::Actor()
	{
		m_actor = NULL;
		m_density = DEFAULT_DENSITY;
		m_aType = DEFAULT_ACTOR_TYPE;
	}

	Actor::Actor(Transform pose, Fl32 density, ActorType aType)
	{
		m_actor = NULL;
		m_pose = pose;
		m_density = density;
		m_aType = aType;
	}

	Actor::Actor(const Actor& param)
	{
		if(param.m_actor)
			*m_actor = *param.m_actor;
		else
			m_actor = NULL;

		m_density = param.m_density;
		m_aType = param.m_aType;
	}

	Actor& Actor::operator=(const Actor& param)
	{
		if(this == &param)
			return *this;
		else
		{
			if(param.m_actor)
				*m_actor = *param.m_actor;
			else
				m_actor = NULL;

			m_density = param.m_density;
			m_aType = param.m_aType;
			return *this;
		}
	}

	Actor::~Actor()
	{
		if(m_actor)
			m_actor->release();
	}

	PxRigidActor* Actor::Get()
	{
		return m_actor;
	}

	Transform Actor::Pose()
	{
		return Get()->getGlobalPose();
	}

	#ifdef _DEBUG
	void Actor::PrintPose() const
	{
		Out(std::to_string(m_actor->getGlobalPose().p.x).c_str());
		Out(" ");
		Out(std::to_string(m_actor->getGlobalPose().p.y).c_str());
		Out(" ");
		Out(std::to_string(m_actor->getGlobalPose().p.z).c_str());
		Out("\n");
	}
	#endif

	/*------------------------------------------------------------------------\
	|							SHAPEACTOR DEFINITIONS							 |
	\-------------------------------------------------------------------------*/
	ShapeActor::ShapeActor(Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType) : Actor(pose, density, aType)
	{
		m_material = cpyMaterial(material);
		m_color = color;
		m_geometry = PxGeometryHolder();
		m_aType = aType;
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
		if(m_shape)
			m_shape->release();
	}

	void ShapeActor::Create()
	{
		void* s = NULL;

		if(m_aType == DynamicActor)
		{
			PxRigidDynamic* ptr = StaticCast(s, PxRigidDynamic*); // Receive Correctly Cast Pointer
			ptr = Physics::PxGetPhysics()->createRigidDynamic(m_pose);
			m_shape = ptr->createShape(m_geometry.any(), *m_material, IDENTITY_TRANS);
			PxRigidBodyExt::setMassAndUpdateInertia(*ptr, m_density);
			m_actor = ptr;
		}
		else
		{
			PxRigidStatic* ptr = StaticCast(s, PxRigidStatic*); // Receive Correctly Cast Pointer
			ptr = Physics::PxGetPhysics()->createRigidStatic(m_pose);
			m_shape = ptr->createShape(m_geometry.any(), *m_material, IDENTITY_TRANS);
			m_actor = ptr;
		}
		if(m_actor)
			m_actor->userData = &m_color;
		else
			Log::Write("Exc: Error creating Box Actor. m_actor is NULL!", ENGINE_LOG);
	}

	PxShape* ShapeActor::GetShape()
	{
		PxShape** buf;
		buf = new PxShape*[1];
		m_actor->getShapes(buf, 1);
		return *buf;
	}



	/*------------------------------------------------------------------------\
	|					COMPOUNDSHAPEACTOR DEFINITIONS							|
	\-------------------------------------------------------------------------*/
	CompoundShapeActor::CompoundShapeActor(int numberOfShapes, Transform pose, Fl32 density, PxMaterial* material, Vec3 color, ActorType aType)
		: Actor(pose, density, aType)
	{
		nShapes = numberOfShapes;
		m_material = cpyMaterial(material);
		m_color = color;
	}
		
	CompoundShapeActor::CompoundShapeActor(const CompoundShapeActor& param)
	{
		m_material = cpyMaterial(param.m_material);
		m_color = param.m_color;
		if(param.m_geometrys)
		{
			m_geometrys = new PxGeometryHolder[param.nShapes];

			for(int i = 0; i < param.nShapes; i++)
				m_geometrys[i] = param.m_geometrys[i];
		}
		else
			m_geometrys = NULL;

	}
		
	CompoundShapeActor& CompoundShapeActor::operator=(const CompoundShapeActor& param)
	{
		if(&param == this)
			return *this;
		else
		{
			Actor::operator=(param);

			if(m_material)
				m_material->release();

			m_material = cpyMaterial(param.m_material);

			if(m_geometrys)
				delete[] m_geometrys;
			if(param.m_geometrys)
			{
				m_geometrys = new PxGeometryHolder[param.nShapes];

				for(int i = 0; i < param.nShapes; i++)
					m_geometrys[i] = param.m_geometrys[i];
			}
			else
				m_geometrys = NULL;

			m_color = param.m_color;

			return *this;
		}
	}

	CompoundShapeActor::~CompoundShapeActor()
	{
		if(m_geometrys)
			delete[] m_geometrys;
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

	Vec3 Box::Dimensions() const
	{
		return m_dimensions;
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

	Fl32 Sphere::Radius() const
	{
		return m_radius;
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