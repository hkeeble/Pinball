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

Pinball::Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY)
	: Game(title, windowWidth, windowHeight, windowPosX, windowPosY)
{
	m_ball = NULL;
	m_ground = NULL;
}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
	// Set State
	gameState = Menu;

	// Camera
	Log::Write("Intializing Camera...\n", ENGINE_LOG);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera = Camera(UP_VECTOR, Vec3(0, 0, 0), Vec3(0, 1, -5.5), DEFAULT_FOV);
	camera.Update(); // Used to initialize camera positions (gluLookAt)

	// Set Perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH)/glutGet(GLUT_INIT_WINDOW_HEIGHT);
	gluPerspective(camera.FOV, r, .1, 100);
	glMatrixMode(GL_MODELVIEW);

	Log::Write("Intializing Colors...\n", ENGINE_LOG);

	// Colors
	Vec3 BallColor		= Vec3(.8f, .8f, .8f);
	Vec3 BoardColor		= Vec3(.4f, .4f, .4f);
	Vec3 BorderColor	= Vec3(0.f, 0.f, 0.f);
	Vec3 ClearColor		= Vec3(.5f, 1.f, 1.f);
	Vec3 PlungerColor	= Vec3(.1f, .1f, 1.f);
	Vec3 GlassColor		= Vec3(.3f, .3f, .3f);

	Log::Write("Intializing Materials...\n", ENGINE_LOG);

	// Materials
	PxMaterial* BoardMaterial   = PHYSICS->createMaterial(0.f, 0.f, .1f);
	PxMaterial* BorderMaterial	= PHYSICS->createMaterial(0.f, 0.f, .2f);
	PxMaterial* BallMaterial	= PHYSICS->createMaterial(0.f, 0.f, .2f);
	Fl32		BallDensity		= 1.f;
	PxMaterial* PlungerMaterial = PHYSICS->createMaterial(0.f, 0.f, .1f);
	Fl32		PlungerDensity	= 2.f;

	// Set Clear Color
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.0);

	Log::Write("Intializing Actors...\n", ENGINE_LOG);

	// Static Actors
	board		 = new Board	 (BoardMaterial,   BoardColor);
	m_border	 = new Border	 (BorderMaterial,  BorderColor);
	m_innerWalls = new InnerWalls(BorderMaterial,  BorderColor); 

	// Dynamic Actors
	m_plunger	 = new Plunger	 (PlungerMaterial, PlungerColor, PlungerDensity);

	// Other Actors (Using primitive actor types)
	// Poses
	Transform BallPose = board->Pose() * Transform(board->Right().x + (board->WallWidth()*2) + BALL_RADIUS*2, board->Dimensions().y*2 + BALL_RADIUS*2, 0); 
	Transform GlassPose = board->Pose() * Transform(Vec3(0, board->WallHeight()*2 + (board->Dimensions().y*2), 0));

	m_ball	= new Sphere(BallPose, BALL_RADIUS, BallDensity, BallColor, BallMaterial);
	m_glass = new Box(GlassPose, board->Dimensions(), 0, GlassColor, BoardMaterial, StaticActor);

	Log::Write("Adding Actors to Scene...\n", ENGINE_LOG);

	// Add Actors to scene (Note: Glass must be added second to prevent rendering)
	m_scene->Add(board);
	m_scene->Add(m_glass);
	m_scene->Add(m_ball);
	m_scene->Add(m_border);
	m_scene->Add(m_innerWalls);
	m_scene->Add(m_plunger);
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

		glutSwapBuffers();
		break;
	}
}

void Pinball::Idle()
{
	switch(gameState)
	{
	case InGame:
		m_scene->Update(m_fps);
		glutPostRedisplay();
		break;
	}
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
	switch(gameState)
	{
	/* Menu Keys */
	case Menu:
		if(key == VK_RETURN)
			gameState = InGame;
		break;
	}

	// Keys applicable to all states
	if(key == VK_ESCAPE)
		exit(0);
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
	if(board)
		delete board;
	if(m_ground)
		delete m_ground;
	if(m_innerWalls)
		delete m_innerWalls;
	//if(m_plunger)
		//delete m_plunger;
}