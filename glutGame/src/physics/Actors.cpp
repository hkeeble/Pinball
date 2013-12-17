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
		m_actor.dynamicActor = nullptr;
		m_actor.staticActor = nullptr;
		m_density = DEFAULT_DENSITY;
		m_aType = DEFAULT_ACTOR_TYPE;
	}

	Actor::Actor(Transform pose, Fl32 density, ActorType aType)
	{
		m_actor.dynamicActor = nullptr;
		m_actor.staticActor = nullptr;
		m_pose = pose;
		m_density = density;
		m_aType = aType;
	}

	Actor::Actor(const Actor& param)
	{
		if(param.m_actor.dynamicActor)
			*m_actor.dynamicActor = *param.m_actor.dynamicActor;
		else
			m_actor.dynamicActor = nullptr;
		if (param.m_actor.staticActor)
			*m_actor.staticActor = *param.m_actor.staticActor;
		else
			m_actor.staticActor = nullptr;

		m_density = param.m_density;
		m_aType = param.m_aType;
	}

	Actor& Actor::operator=(const Actor& param)
	{
		if(this == &param)
			return *this;
		else
		{
			if (param.m_actor.dynamicActor)
				*m_actor.dynamicActor = *param.m_actor.dynamicActor;
			else
				m_actor.dynamicActor = nullptr;
			if (param.m_actor.staticActor)
				*m_actor.staticActor = *param.m_actor.staticActor;
			else
				m_actor.staticActor = nullptr;

			m_density = param.m_density;
			m_aType = param.m_aType;
			return *this;
		}
	}

	Actor::~Actor()
	{
		if (m_aType == DynamicActor)
			PX_RELEASE(m_actor.dynamicActor);
		if (m_aType == StaticActor)
			PX_RELEASE(m_actor.staticActor);
	}

	ActorUnion Actor::Get()
	{
		return m_actor;
	}

	Transform Actor::Pose()
	{
		if (m_actor.dynamicActor)
			return m_actor.dynamicActor->getGlobalPose();
		else if (m_actor.staticActor)
			return m_actor.staticActor->getGlobalPose();
	}

	#ifdef _DEBUG
	void Actor::PrintPose() const
	{
		if (m_actor.dynamicActor)
		{
			Out(std::to_string(m_actor.dynamicActor->getGlobalPose().p.x).c_str());
			Out(" ");
			Out(std::to_string(m_actor.dynamicActor->getGlobalPose().p.y).c_str());
			Out(" ");
			Out(std::to_string(m_actor.dynamicActor->getGlobalPose().p.z).c_str());
			Out("\n");
		}
		else if (m_actor.staticActor)
		{
			Out(std::to_string(m_actor.staticActor->getGlobalPose().p.x).c_str());
			Out(" ");
			Out(std::to_string(m_actor.staticActor->getGlobalPose().p.y).c_str());
			Out(" ");
			Out(std::to_string(m_actor.staticActor->getGlobalPose().p.z).c_str());
			Out("\n");
		}
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
			PX_RELEASE(m_material);
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
		PX_RELEASE(m_material);
		PX_RELEASE(m_shape);
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
			m_actor.dynamicActor = ptr;
		}
		else
		{
			PxRigidStatic* ptr = StaticCast(s, PxRigidStatic*); // Receive Correctly Cast Pointer
			ptr = Physics::PxGetPhysics()->createRigidStatic(m_pose);
			m_shape = ptr->createShape(m_geometry.any(), *m_material, IDENTITY_TRANS);
			m_actor.staticActor = ptr;
		}
		if (m_actor.dynamicActor)
			m_actor.dynamicActor->userData = &m_color;
		else if (m_actor.staticActor)
			m_actor.staticActor->userData = &m_color;
		else
			Log::Write("Exc: Error creating Box Actor. m_actor is NULL!", ENGINE_LOG);
	}

	PxShape* ShapeActor::GetShape()
	{
		PxShape** buf;
		buf = new PxShape*[1];
		if (m_aType == DynamicActor)
			m_actor.dynamicActor->getShapes(buf, 1);
		else
			m_actor.staticActor->getShapes(buf, 1);
		return *buf;
	}

	void ShapeActor::SetShapeFlag(PxShapeFlag::Enum flag, bool value)
	{
		GetShape()->setFlag(flag, value);
	}

	void ShapeActor::IsTrigger(bool value)
	{
		PxRigidDynamic* dyn = nullptr;
		PxRigidStatic* st = nullptr;

		if (m_aType == DynamicActor)
		{
			dyn = m_actor.dynamicActor;
			if (value)
				dyn->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			else
				dyn->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		}

		if (value)
		{
			GetShape()->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
			GetShape()->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
		}
		else
		{
			GetShape()->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
			GetShape()->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false);
		}
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

			PX_RELEASE(m_material);

			m_material = cpyMaterial(param.m_material);

			RELEASE_MULTI(m_geometrys);

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
		RELEASE_MULTI(m_geometrys);
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
	|						CONVEX MESH ACTOR DEFINITIONS						|
	\-------------------------------------------------------------------------*/
	ConvexMeshActor::ConvexMeshActor(VertexSet verts, Transform pose, Fl32 density, const Vec3& color, PxMaterial* material, Vec3 scale, ActorType aType)
		: ShapeActor(pose, density, material, color, aType)
	{
		m_scale = scale;
		m_pose = m_pose * Transform(Quat(DEG2RAD(90), Vec3(1, 0, 0))); // Default Pose Rotation
		
		PxConvexMesh* mesh = CreateConvexMesh(verts.GetVerts(), verts.NumberOfVerts(), m_scale);
		m_geometry.storeAny(PxConvexMeshGeometry(mesh));

		Create();
	}

	ConvexMeshActor::ConvexMeshActor(const ConvexMeshActor& param) : ShapeActor(param)
	{
		m_scale = param.m_scale;
	}

	ConvexMeshActor& ConvexMeshActor::operator=(const ConvexMeshActor& param)
	{
		if (this == &param)
			return *this;
		else
		{
			ShapeActor::operator=(param);
			return *this;
		}
	}
	ConvexMeshActor::~ConvexMeshActor()
	{

	}

	void ConvexMeshActor::Create()
	{
		void* s = nullptr;
		PxShape* shape = nullptr;

		if (m_aType == DynamicActor)
		{
			PxRigidDynamic* ptr = StaticCast(s, PxRigidDynamic*); // Receive Correctly Cast Pointer
			ptr = PHYSICS->createRigidDynamic(m_pose);
			shape = ptr->createShape(m_geometry.convexMesh(), *m_material);
			PxRigidBodyExt::setMassAndUpdateInertia(*ptr, m_density);
			m_actor.dynamicActor = ptr;
			m_actor.dynamicActor->userData = &m_color;
		}
		else
		{
			PxRigidStatic* ptr = StaticCast(s, PxRigidStatic*); // Receive Correctly Cast Pointer
			ptr = PHYSICS->createRigidStatic(m_pose);
			shape = ptr->createShape(m_geometry.convexMesh(), *m_material);
			m_actor.staticActor = ptr;
			m_actor.staticActor->userData = &m_color;
		}
	}

	ConvexMeshActor* ConvexMeshActor::CreateWedge(Transform pose, Fl32 density, const Vec3& color, PxMaterial* material, Vec3 scale, ActorType aType)
	{
		int nVerts = sizeof(wedge_verts) / sizeof(Vec3);
		Vec3* verts = new Vec3[nVerts];
		std::copy(std::begin(wedge_verts), std::end(wedge_verts), verts);
		return new ConvexMeshActor(VertexSet(verts, nVerts), pose, density, color, material, scale, aType);
	}

	ConvexMeshActor* ConvexMeshActor::CreateHexagon(Transform pose, Fl32 density, const Vec3& color, PxMaterial* material, Vec3 scale, ActorType aType)
	{
		int nVerts = 48;
		Vec3* totalVerts = new Vec3[nVerts];
		Vec3* vertsBottom = new Vec3[12];
		Vec3* vertsTop = new Vec3[12];

		int count = 0;
		for (int i = 0; i < 360; i += 60)
		{
			Fl32 heading = DEG2RAD(i);
			vertsBottom[count] = Vec3(cos(heading) * 1, 0, sin(heading) * 1); // Assumed Radius 1
			count++;
			vertsBottom[count] = Vec3(0, 0, 0);
			count++;
		}

		for (int i = 0; i < 12; i += 2)
		{
			vertsTop[i] = Vec3(vertsBottom[i].x, 1, vertsBottom[i].z);
			vertsTop[i + 1] = Vec3(0, 0, 0);
		}

		for (int i = 0; i < nVerts; i++)
		{
			int c = i;
			if (c >= nVerts / 2)
				c -= nVerts / 2;

			if (i % 2 == 0)
				totalVerts[i] = vertsTop[c];
			else
				totalVerts[i] = vertsBottom[c];
		}
	
		return new ConvexMeshActor(VertexSet(totalVerts, nVerts), pose, density, color, material, scale, aType);
	}

	ConvexMeshActor* ConvexMeshActor::CreatePyramid(Transform pose, Fl32 density, const Vec3& color, PxMaterial* material, Vec3 scale, ActorType aType)
	{
		int nVerts = sizeof(pyramid_verts) / sizeof(Vec3);
		Vec3* verts = new Vec3[nVerts];
		std::copy(std::begin(pyramid_verts), std::end(pyramid_verts), verts);
		return new ConvexMeshActor(VertexSet(verts, nVerts), pose, density, color, material, scale, aType);
	}
}