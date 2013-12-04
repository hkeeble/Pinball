/*-------------------------------------------------------------------------\
| File: PINBALL.CPP															|
| Desc: Provides implementations for a pinball game.						|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "boardObjects.h"
#include "globals.h"

// Assign Constants
const int MAX_NUM_ACTOR_SHAPES = 128;

Board* Pinball::board;

Pinball::Pinball(std::string title, int windowWidth, int windowHeight)
	: Game(title, windowWidth, windowHeight)
{
	m_ball = nullptr;
	m_glass = nullptr;
	m_border = nullptr;
	m_innerWalls = nullptr;
	m_plunger = nullptr;
	m_flippers = nullptr;
	m_actors = std::vector<Actor*>();
}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
	// Set FPS
	m_fps = 1.f / FPS;

	// Set State
	gameState = Menu;

	// Camera
	Log::Write("Intializing Camera...\n", ENGINE_LOG);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera = { UP_VECTOR, Vec3(0, 0, 0), Vec3(0, 1, -5.5), DEFAULT_FOV };
	camera.Update(); // Used to initialize camera positions (gluLookAt)
	
	// Set Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH)/glutGet(GLUT_INIT_WINDOW_HEIGHT);
	gluPerspective(camera.FOV, r, .1, 100);
	glMatrixMode(GL_MODELVIEW);

	const Vec3 ClearColor = Vec3(.5f, 1.f, 1.f);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.0);

	InitBoard();
	InitInnerWalls();
	InitFlippers();
	InitBall();
	InitPlunger();

	AddActors();

	InitJoints();
}

void Pinball::InitJoints()
{
	// Plunger Spring
	AddDistanceJoint(m_plunger->Get().dynamicActor, PxTransform::createIdentity(), NULL, m_plunger->Get().dynamicActor->getGlobalPose(),
		PxDistanceJointFlag::eSPRING_ENABLED, 1000.f, 1.f);
	m_plunger->SetKinematic(true);
}

void Pinball::InitBall()
{
	const Vec3 BallColor = Vec3(.8f, .8f, .8f);
	PxMaterial* BallMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);
	const Fl32 BallDensity = 1.f;
	Transform BallPose = board->Pose() * Transform(board->Right().x + (board->WallWidth() * 2) + BALL_RADIUS * 2, board->Dimensions().y * 2 + BALL_RADIUS * 2, 0);
	m_ball = new Sphere(BallPose, BALL_RADIUS, BallDensity, BallColor, BallMaterial);
	m_actors.push_back(m_ball);
}

void Pinball::InitBoard()
{
	const Vec3 BoardColor = Vec3(.4f, .4f, .4f);
	const Vec3 BorderColor = Vec3(0.f, 0.f, 0.f);

	PxMaterial* BoardMaterial = PHYSICS->createMaterial(0.f, 0.f, .1f);
	PxMaterial* BorderMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);

	// board and border
	board = new Board(BoardMaterial, BoardColor);
	m_border = new Border(BorderMaterial, BorderColor);

	// Glass
	Transform GlassPose = board->Pose() * Transform(Vec3(0, board->WallHeight() * 2 + (board->Dimensions().y * 2), 0));
	m_glass = new Box(GlassPose, board->Dimensions(), 0, Vec3(0, 0, 0), BoardMaterial, StaticActor);
	
	m_actors.push_back(board);
	m_actors.push_back(m_glass);
	m_actors.push_back(m_border);
}

void Pinball::InitInnerWalls()
{
	const Vec3 WallColor = Vec3(0.f, 0.f, 0.f);
	PxMaterial* WallMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);

	m_innerWalls = new InnerWalls(WallMaterial, WallColor);

	m_actors.push_back(m_innerWalls);
}

void Pinball::InitFlippers()
{
	// Flippers Data
	const Vec3 FlipperColor = Vec3(.5f, 0.f, 0.f);
	const Fl32 FlipperDensity = 2.f;
	PxMaterial* FlipperMaterial = PHYSICS->createMaterial(0.f, 0.f, 0.1f);

	// Flipper Positions
	const Transform lftFPos = Transform(board->Bottom() + Vec3(0.f, 0.f, 0.f), Quat(DEG2RAD(-35), Vec3(0, 1, 0)));
	const Transform rgtFPos = Transform(board->Bottom() - Vec3(0.1f, 0, 1.13f), Quat(DEG2RAD(-35), Vec3(0, 1, 0)));

	// Flipper Joint Poses (relative)
	const Transform lftFJPos = Transform(0.f, 1.f, 0.f);
	const Transform rgtFJPos = Transform(0.01f, 0.f, 0.f);

	// Create Flippers
	Flipper* m_lftFlipper = new Flipper(lftFPos, FlipperMaterial, FlipperColor, FlipperDensity, lftFJPos);
	Flipper* m_rgtFlipper = new Flipper(rgtFPos, FlipperMaterial, FlipperColor, FlipperDensity, rgtFJPos);

	// Create Flippers Object
	m_flippers = new Flippers(m_rgtFlipper, m_lftFlipper);

	// Add Flippers to Actors Vector
	m_actors.push_back(m_flippers->GetLeft());
	m_actors.push_back(m_flippers->GetRight());
}

void Pinball::InitPlunger()
{
	PxMaterial* PlungerMaterial = PHYSICS->createMaterial(0.f, 0.f, .1f);
	const Vec3 PlungerColor = Vec3(.1f, .1f, 1.f);
	const Fl32 PlungerDensity = 2.f;

	m_plunger = new Plunger(PlungerMaterial, PlungerColor, PlungerDensity);

	m_actors.push_back(m_plunger);
}

void Pinball::AddActors()
{
	for (std::vector<Actor*>::iterator iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		Actor* act = *iter;
		m_scene->Add(act);
	}
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(gameState)
	{
	case Menu:
		break;
	case InGame:
		// Update Camera
		camera.Update();

		// Render Scene
		std::vector<physx::PxRigidActor*> actors = m_scene->GetActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC | physx::PxActorTypeSelectionFlag::eRIGID_STATIC); // get scene actors
	
		int nbActors = actors.size();
		PxShape* shapes[MAX_NUM_ACTOR_SHAPES]; // Pointer to current actor shapes

		if(actors.size())
		{
			for(int i = 0; i < nbActors; i++)
			{
				if(i != GLASS_ATR_IDX) // Don't Render the glass
				{
					PxU32 nbShapes = actors[i]->getNbShapes();
					if(nbShapes <= MAX_NUM_ACTOR_SHAPES)
					{
						actors[i]->getShapes(shapes, nbShapes);

						bool isSleeping = actors[i]->isRigidDynamic() && actors[i]->isRigidDynamic()->isSleeping(); // Check if actor is sleeping

						for(int j = 0; j < nbShapes; j++)
						{
							Transform p = PxShapeExt::getGlobalPose(*shapes[j], *actors[i]);
							PxGeometryHolder h = shapes[j]->getGeometry();

							Mat44 pose(p); // Create Matrix from vector

							glPushMatrix();
							glMultMatrixf((Fl32*)&pose); // Multiply current matrix by pose

							Vec3 aColor = *((Vec3*)actors[i]->userData);
							glColor3f(aColor.x, aColor.y, aColor.z);

							if(h.getType() == PxGeometryType::ePLANE)
								glDisable(GL_LIGHTING);

							Game::RenderGeometry(h);

							if(h.getType() == PxGeometryType::ePLANE)
								glEnable(GL_LIGHTING);

							glPopMatrix();

							Vec3 defCol = DEFAULT_COLOR;
							glColor3f(defCol.x, defCol.y, defCol.z);
						}
					}
					else
						Log::Write("Exc: Too many shapes in actor!\n", ENGINE_LOG);
				}
			}
		}

		switch (gameState)
		{
		case InGame:
			m_scene->UpdatePhys(m_fps);
			break;
		}

		glutSwapBuffers();
		Game::Render();
		break;
	}
}

void Pinball::Idle()
{

}

void Pinball::Reshape(int width, int height)
{
	Game::Reshape(width, height);
}

void Pinball::MouseButton(int button, int state, int x, int y)
{

}

void Pinball::MouseMove(int x, int y)
{

}

void Pinball::KeyboardDown(unsigned char key, int x, int y)
{
	switch (gameState)
	{
	/* Menu Keys */
	case Menu:
		if (key == VK_RETURN)
		{
			gameState = InGame;
			glutPostRedisplay();
		}
		break;
	/* InGame Keys */
	case InGame:
		if (key == VK_SPACE)
			m_plunger->SetKinematicTarget(Transform(Vec3(0, 0, -.02f)));
		if (key == VK_RETURN)
			m_plunger->Reset();
	}

	// Keys applicable to all states
	if(key == VK_ESCAPE)
		exit(0);
}

void Pinball::KeyboardUp(unsigned char key, int x, int y)
{
	switch(gameState)
	{
	case InGame:
		if(key == VK_SPACE)
			m_plunger->SetKinematic(false);
	}
}

void Pinball::SpecKeyboardDown(int key, int x, int y)
{
	switch (gameState)
	{
	case Menu:
		break;
	case InGame:
		if (key == GLUT_KEY_LEFT)
			m_flippers->FlipLeft();
		if (key == GLUT_KEY_RIGHT)
			m_flippers->FlipRight();
		break;
	}
}

void Pinball::SpecKeyboardUp(int key, int x, int y)
{

}

void Pinball::Exit()
{
	Game::Exit();

	if(m_ball)
		delete m_ball;
	if(board)
		delete board;
	if(m_innerWalls)
		delete m_innerWalls;
	//if(m_plunger)
		//delete m_plunger;
}