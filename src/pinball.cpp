/*-------------------------------------------------------------------------\
| File: PINBALL.CPP															|
| Desc: Provides implementations for a pinball game.						|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "globals.h"

#define BoardMaterial PHYSICS->createMaterial(0.f, 0.f, .1f)
#define BorderMaterial PHYSICS->createMaterial(0.f, 0.f, 0.2f)
#define BallMaterial PHYSICS->createMaterial(0.f, 0.f, .2f)

// Assign Constants
const int MAX_NUM_ACTOR_SHAPES = 128;

Pinball::Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY)
	: Game(title, windowWidth, windowHeight, windowPosX, windowPosY),
	 // Initialize Constants
	 m_ballRadius		(0.039375f),
	 m_fallHoleWidth	(m_ballRadius*2),
	 m_plungerLaneWidth	(m_fallHoleWidth),
	 m_boardDimensions	(Vec3(1.6f, 0.05f, 3.0f)),
	 m_boardPose		(Vec3(0, 0, 1.f), Quat(DEG2RAD(-25), Vec3(1, 0, 0))),
	 m_wallHeight		(0.1f),
	 m_wallWidth		(0.05f)
{
	m_ball = NULL;
	m_board = NULL;
	m_ground = NULL;
}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
	// Camera
	Log::Write("Intializing Camera...\n", ENGINE_LOG);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera = Camera(UP_VECTOR, Vec3(0, 0, 1), Vec3(0, 5, -1.5), DEFAULT_FOV);
	camera.Update();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH)/glutGet(GLUT_INIT_WINDOW_HEIGHT);
	gluPerspective(camera.FOV, r, .1, 100);
	glMatrixMode(GL_MODELVIEW);

	Log::Write("Intializing Actors...\n", ENGINE_LOG);

	// Colors
	Color ballC = Color(0.8, 0.8, 0.8);
	Color brdC  = Color(0.4, 0.4, 0.4);
	Color bdrC  = Color(0, 0, 0);
	Color cc	= Color(0.5, 1, 1);

	// Set Clear Color
	glClearColor(cc.r, cc.g, cc.b, 1.0);

	// Actors
	m_board = new Box(m_boardPose, m_boardDimensions, 1.f, Vec3(brdC.r, brdC.g, brdC.b), BoardMaterial, StaticActor);

	m_ball = new Sphere(Transform(-m_boardDimensions.x+m_ballRadius+0.05f, m_boardDimensions.y+m_ballRadius, m_boardPose.p.z), m_ballRadius, 1.f, Vec3(ballC.r, ballC.g, ballC.b), BallMaterial);

	m_border = new Border(BorderMaterial, Vec3(bdrC.r, bdrC.g, bdrC.b), *m_board, m_fallHoleWidth, m_plungerLaneWidth, m_wallHeight, m_wallWidth);

	m_glass = new Box(Transform(Vec3(0, m_boardDimensions.y+(m_ballRadius*2)+(m_boardDimensions.y)+0.01f, m_boardPose.p.z-((m_boardDimensions.y/2)/2)), m_boardPose.q), m_boardDimensions, 1.f, Vec3(.5f, .5f, .5f), BoardMaterial, StaticActor);

	m_innerWalls = new InnerWalls(BorderMaterial, Vec3(bdrC.r, bdrC.g, bdrC.b), *m_board, m_fallHoleWidth, m_plungerLaneWidth, m_wallHeight, m_wallWidth); 

	// Add Actors to scene (Note: Glass must be added second)
	m_scene->Add(m_board);
	m_scene->Add(m_glass);
	m_scene->Add(m_ball);
	m_scene->Add(m_border);
	m_scene->Add(m_innerWalls);
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	glutSwapBuffers();
}

void Pinball::Idle()
{
	Game::Idle();

#ifdef _DEBUG
	//m_ball->PrintPose();
#endif
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

}

void Pinball::KeyboardUp(unsigned char key, int x, int y)
{

}

void Pinball::SpecKeyboardDown(int key, int x, int y)
{

}

void Pinball::SpecKeyboardUp(int key, int x, int y)
{

}

void Pinball::Exit()
{
	Game::Exit();

	if(m_ball)
		delete m_ball;
	if(m_board)
		delete m_board;
	if(m_ground)
		delete m_ground;
}