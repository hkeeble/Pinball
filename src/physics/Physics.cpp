/*-------------------------------------------------------------------------\
| File: PHYSICS.CPP															|
| Desc: Implementations for a namespace used to simulate physics with		|
|		NVIDIA PhysX.														|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "Physics.h"

namespace Physics
{
	// -- Callbacks --
	PxDefaultAllocator defaultAllocatorCallBack;
	ErrorCallback errorCallback;

	// -- PhysX Objects --
	PxFoundation* foundation = NULL;
	PxPhysics* physics = NULL;

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

	std::vector<PxRigidActor*> Scene::GetActors(PxActorTypeSelectionFlags flags) const
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
		actor->Create();
		m_scene->addActor(*actor->Get());
	}

	/*-------------------------------------------------------------------------\
	|							ACTOR DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Actor::Actor()
	{
		m_actor = NULL;
		m_pose = IDENTITY_TRANS;
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

		m_pose = param.m_pose;
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

			m_pose = param.m_pose;
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

	PxActor* Actor::Get()
	{
		return m_actor;
	}

	#ifdef _DEBUG
	void Actor::PrintPose() const
	{
		Out(std::to_string(m_pose.p.x).c_str());
		Out(std::to_string(m_pose.p.y).c_str());
		Out(std::to_string(m_pose.p.z).c_str());
		Out("\n");
	}
	#endif
}