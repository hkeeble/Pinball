/*-------------------------------------------------------------------------\
| File: PHYSICS.CPP															|
| Desc: Implementations for a namespace used to simulate physics with		|
|		NVIDIA PhysX.														|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "physics\Physics.h"
#include "Actors.h"

namespace Physics
{
	// -- Callbacks --
	PxDefaultAllocator defaultAllocatorCallBack;
	PxDefaultErrorCallback defaultErrorCallBack;
	ErrorCallback errorCallback;

	// -- PhysX Objects --
	PxFoundation* foundation = nullptr;
	PxPhysics* physics = nullptr;
	PxCooking* cooking = nullptr;

	// Visual Debugger
#ifdef _DEBUG
	debugger::comm::PvdConnection* vd_connection = 0;
#endif

	/*-------------------------------------------------------------------------\
	|						PHYSICS INITIALIZATION								|
	\-------------------------------------------------------------------------*/
	void PxInit()
	{
		Log::Write("Initializing PhysX version ", ENGINE_LOG);
		Log::Write((std::to_string(PX_PHYSICS_VERSION_MAJOR) + "." + std::to_string(PX_PHYSICS_VERSION_MINOR) + "." + std::to_string(PX_PHYSICS_VERSION_BUGFIX)).c_str(), ENGINE_LOG);
		Log::Write("...\n", ENGINE_LOG);

		defaultErrorCallBack = PxDefaultErrorCallback();
		defaultAllocatorCallBack = PxDefaultAllocator();

		Log::Write("\tCreating foundation...\n", ENGINE_LOG);

		if (!foundation)
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallBack, errorCallback);

		Log::Write("\tCreating physics object...\n", ENGINE_LOG);

		if (!physics)
			physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());

		Log::Write("\tCreating cooking object...\n", ENGINE_LOG);

		if (!cooking)
			cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
	}

	// Release Physics from memory
	void PxRelease()
	{
		Log::Write("Releasing PhysX Resources...\n", ENGINE_LOG);

		PX_RELEASE(physics);
		PX_RELEASE(foundation);
	}

	// Get PhysX Objects
	PxPhysics* PxGetPhysics()
	{
		return physics;
	}

	/*-------------------------------------------------------------------------\
	|							UTILITY DEFINITIONS								|
	\-------------------------------------------------------------------------*/

	PxCooking* PxGetCooking()
	{
		return cooking;
	}

	PxMaterial* cpyMaterial(PxMaterial* src)
	{
		if(src)
			return Physics::PxGetPhysics()->createMaterial(src->getStaticFriction(),
				src->getDynamicFriction(), src->getRestitution());
		else
		{
			Log::Write("Exc: Cannot copy material, source is NULL!\n", ENGINE_LOG);
			return NULL;
		}
	}

	PxShape* cpyShape(PxShape* src)
	{
		PxBoxGeometry bgeo;
		PxSphereGeometry sgeo;

		if(src)
		{
			switch(src->getGeometryType())
			{
			case physx::PxGeometryType::eBOX:
				src->getBoxGeometry(bgeo);
				return Physics::PxGetPhysics()->createShape(bgeo, *DEFAULT_MATERIAL);
				break;
			case physx::PxGeometryType::eSPHERE:
				src->getSphereGeometry(sgeo);
				return Physics::PxGetPhysics()->createShape(sgeo, *DEFAULT_MATERIAL);
				break;
			default:
				Log::Write("Err: Error copying shape, type not recognized!\n", ENGINE_LOG);
				return NULL;
			}
		}
		else
		{
			Log::Write("Exc: Cannot copy shape, source is NULL!\n", ENGINE_LOG);
			return NULL;
		}
	}

	/* Adds a distance joint between two rigid actors */
	void AddDistanceJoint(PxRigidActor* actor0, PxTransform& localFrame0, PxRigidActor* actor1, PxTransform& localFrame1,
		PxDistanceJointFlag::Enum flags, PxReal stiffness, PxReal damping)
	{
		PxDistanceJoint* joint = PxDistanceJointCreate(*PHYSICS, actor0, localFrame0, actor1, localFrame1);
		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION,true);
		joint->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
		joint->setStiffness(stiffness);
		joint->setDamping(damping);
	}

	/* Create a Convex Mesh from the given vertices and scale */
	PxConvexMesh* CreateConvexMesh(Vec3* const verts, const int& nVerts, const Vec3& scale)
	{
		PxConvexMeshDesc desc;
		desc.points.count = nVerts;
		desc.points.stride = sizeof(Vec3);

		// Scale Vertices
		for (int i = 0; i < nVerts; i++)
			verts[i] = Vec3(verts[i].x * scale.x, verts[i].y * scale.y, verts[i].z * scale.z);

		desc.points.data = verts;
		desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		desc.vertexLimit = VERTEX_LIMIT;

		return Cook(desc);
	}

	/* Cooks a given convex mesh description */
	PxConvexMesh* Cook(const PxConvexMeshDesc& desc)
	{
		PxDefaultMemoryOutputStream stream;
		PxGetCooking()->cookConvexMesh(desc, stream);
		PxDefaultMemoryInputData input(stream.getData(), stream.getSize());
		return physics->createConvexMesh(input);
	}

	/*-------------------------------------------------------------------------\
	|						EVENT CALLBACK DEFINITIONS							|
	\-------------------------------------------------------------------------*/

	SimulationEventCallback::SimulationEventCallback()
	{
		m_isTriggered = false;
	}

	bool SimulationEventCallback::IsTriggered()
	{
		return m_isTriggered;
	}

	SimulationEventCallback::~SimulationEventCallback()
	{

	}

	void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		for (int i = 0; i < count; i++)
		{
			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				m_isTriggered = true;
			if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
				m_isTriggered = false;

			PxRigidActor* ball = pairs[i].otherActor;
			if (ball->isRigidDynamic())
			{
				PxRigidDynamic* dyn = (PxRigidDynamic*)ball;
				Vec3 lvel = dyn->getAngularVelocity() * 500;
				dyn->addForce(-dyn->getLinearVelocity());
			}
		}
	}

	/*-------------------------------------------------------------------------\
	|							SCENE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Scene::Scene()
	{
		m_scene = nullptr;
		m_pause = false;
		m_eventCallback = nullptr;
	}

	Scene::~Scene()
	{
		PX_RELEASE(m_scene);
		RELEASE(m_eventCallback);
	}

	void Scene::Init()
	{
		Log::Write("Initializing Game Scene...\n", ENGINE_LOG);

		PxSceneDesc sceneDesc(PxGetPhysics()->getTolerancesScale());
		
		if(!sceneDesc.filterShader)
			sceneDesc.filterShader = PxDefaultSimulationFilterShader;

		if(!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
			sceneDesc.cpuDispatcher = mCpuDispatcher;
		}

		m_scene = physics->createScene(sceneDesc);

		m_scene->setGravity(Vec3(0.0f, -8.81f, 0.0f));

		m_pause = false;

		// Initialize visual debugger
#ifdef _DEBUG
		if (!vd_connection)
			vd_connection = PxVisualDebuggerExt::createConnection(physics->getPvdConnectionManager(), 
			"localhost", 5425, 100, PxVisualDebuggerExt::getAllConnectionFlags());

		// Visualization
		m_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
		m_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
#endif
}

	void Scene::UpdatePhys(Fl32 deltaTime)
	{
		if(!m_pause)
		{
			m_scene->simulate (deltaTime);
			m_scene->fetchResults(true);
		}
		else
			return;
	}

	void Scene::SetEventCallback(SimulationEventCallback* eventCallback)
	{
		m_scene->setSimulationEventCallback(eventCallback);
		m_eventCallback = eventCallback;
	}

	SimulationEventCallback* Scene::GetSimulationEventCallback()
	{
		return m_eventCallback;
	}

	bool Scene::IsPaused() const
	{
		return m_pause;
	}

	void Scene::TogglePause()
	{
		m_pause = !m_pause;
	}

	std::vector<PxRigidActor*> Scene::GetActors(PxActorTypeSelectionFlags flags, bool rendering) const
	{
		int numOfActors = m_scene->getNbActors(flags);
		std::vector<PxRigidActor*> atrs(numOfActors);
		if(numOfActors)
			m_scene->getActors(flags, (PxActor**)&atrs[0], numOfActors);
		return atrs;
	}

	void Scene::Add(Actor* actor)
	{
		Log::Write("\tAdding Actor to scene...\n", ENGINE_LOG);
		if(actor->Get().staticActor == nullptr && actor->Get().dynamicActor == nullptr)
			actor->Create();
		if (actor->Get().dynamicActor)
			m_scene->addActor(*actor->Get().dynamicActor);
		else
			m_scene->addActor(*actor->Get().staticActor);
	}
}