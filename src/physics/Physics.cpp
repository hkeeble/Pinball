/*-------------------------------------------------------------------------\
| File: PHYSICS.CPP															|
| Desc: Implementations for a namespace used to simulate physics with		|
|		NVIDIA PhysX.														|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "Physics.h"
#include "Actors.h"

namespace Physics
{
	// -- Callbacks --
	PxDefaultAllocator defaultAllocatorCallBack;
	ErrorCallback errorCallback;

	// -- PhysX Objects --
	PxFoundation* foundation = NULL;
	PxPhysics* physics = NULL;
	// Visual Debugger
#ifdef _DEBUG
	debugger::comm::PvdConnection* vd_connection = 0;
#endif

	/*-------------------------------------------------------------------------\
	|						PHYSICS INITIALIZATION								|
	\-------------------------------------------------------------------------*/
	void PxInit()
	{
		Log::Write("Initializing PhysX...\n", ENGINE_LOG);

		if(!foundation)
			foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallBack, errorCallback);

		if(!physics)
			physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
	}

	// Release Physics from memory
	void PxRelease()
	{
		Log::Write("Releasing PhysX Resources...\n", ENGINE_LOG);

		if (physics)
			physics->release();
		if (foundation)
			foundation->release();
	}

	// Get PhysX Objects
	PxPhysics* PxGetPhysics()
	{
		return physics;
	}

	/*-------------------------------------------------------------------------\
	|							UTILITY DEFINITIONS								|
	\-------------------------------------------------------------------------*/

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
		joint->setDistanceJointFlags(flags);
		joint->setStiffness(stiffness);
		joint->setDamping(damping);
	}

	/*-------------------------------------------------------------------------\
	|							SCENE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Scene::Scene()
	{
		m_scene = NULL;
		m_pause = false;
	}

	Scene::~Scene()
	{
		if(m_scene)
			m_scene->release();
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

		// Initialize visual debugger
#ifdef _DEBUG
		if (!vd_connection)
			vd_connection = PxVisualDebuggerExt::createConnection(physics->getPvdConnectionManager(), 
			"localhost", 5425, 100, PxVisualDebuggerExt::getAllConnectionFlags());
#endif

		m_scene = physics->createScene(sceneDesc);

		m_scene->setGravity(Vec3(0.0f, -8.81f, 0.0f));

		m_pause = false;
	}

	void Scene::Update(Fl32 deltaTime)
	{
		if(!m_pause)
		{
			m_scene->simulate(deltaTime);
			m_scene->fetchResults(true);
		}
		else
			return;
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
		Log::Write("Adding Actor to scene...\n", ENGINE_LOG);
		if(!actor->Get())
			actor->Create();
		m_scene->addActor(*actor->Get());

		#ifdef _DEBUG
		Out("Actor Added at position: ");
		if(actor);
			actor->PrintPose();
		#endif
	}
}