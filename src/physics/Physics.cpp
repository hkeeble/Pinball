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
	PxMaterial* defaultMaterial = NULL;

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

		if(!defaultMaterial)
			defaultMaterial = physics->createMaterial(0.0f, 0.0f, 0.0f);
	}

	// Release Physics from memory
	void PxRelease()
	{
		Log::Write("Releasing PhysX Resources...\n", ENGINE_LOG);

		if (defaultMaterial)
			defaultMaterial->release();
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

	PxMaterial* PxGetDefaultMaterial()
	{
		return defaultMaterial;
	}

	/*-------------------------------------------------------------------------\
	|							SCENE DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
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

		m_scene->setGravity(PxVec3(0.0f, -8.81f, 0.0f));

		m_pause = false;
	}

	void Scene::Update(PxReal deltaTime)
	{
		if(!m_pause)
			m_scene->simulate(deltaTime);
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

	std::vector<PxRigidActor*> Scene::GetActors() const
	{
		int numOfActors = m_scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC);
		std::vector<PxRigidActor*> atrs;
		if(numOfActors)
			m_scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC, (PxActor**)&atrs[0], numOfActors);
		return atrs;
	}

	/*-------------------------------------------------------------------------\
	|							ACTOR DEFINITIONS								|
	\-------------------------------------------------------------------------*/
	Actor::Actor()
	{

	}

	Actor::~Actor()
	{

	}

	PxActor* Actor::Get()
	{
		return m_actor;
	}
}