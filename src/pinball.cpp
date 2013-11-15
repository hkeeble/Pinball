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

Pinball::Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY)
	: Game(title, windowWidth, windowHeight, windowPosX, windowPosY)
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
	camera = Camera(UP_VECTOR, Vec3(0, 0, 1), CAMERA_POS, DEFAULT_FOV);
	camera.Update();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH)/glutGet(GLUT_INIT_WINDOW_HEIGHT);
	gluPerspective(camera.FOV, r, .1, 100);
	glMatrixMode(GL_MODELVIEW);

	Log::Write("Intializing Actors...\n", ENGINE_LOG);

	// Colors
	Color cc = CLEAR_COLOR;
	Color brdC = BOARD_COLOR;
	Color ballC = BALL_COLOR;
	Color bdrC = BORDER_COLOR;

	// Clear Color
	glClearColor(cc.r, cc.g, cc.b, 1.0);

	// Actors
	m_board = new Box(Transform(Vec3(0, -(BRD_HEIGHT/2), BRD_Z_OFFSET), Quat(BRD_ROT, Vec3(1, 0, 0))), Vec3(BRD_WIDTH, BRD_HEIGHT, BRD_LENGTH),
		1.f, Vec3(brdC.r, brdC.g, brdC.b), BoardMaterial, StaticActor);

	m_glass = new Box(Transform(Vec3(0, GLASS_HEIGHT, BRD_Z_OFFSET-GLASS_HEIGHT/2), Quat(BRD_ROT, Vec3(1, 0, 0))), Vec3(BRD_WIDTH, BRD_HEIGHT, BRD_LENGTH),
		1.f, Vec3(.5f, .5f, .5f), BoardMaterial, StaticActor);

	m_ball = new Sphere((PxTransform)Transform(0, -(BRD_HEIGHT/2)+(BALL_RADIUS+0.01f), BRD_Z_OFFSET), BALL_RADIUS, 1.f, Vec3(ballC.r, ballC.g, ballC.b), BallMaterial);

	m_border = new Border(4, IDENTITY_TRANS, 1.f, BorderMaterial, Vec3(bdrC.r, bdrC.g, bdrC.b), StaticActor);

	// Add Actors to scene (Note: Glass must be added second)
	m_scene->Add(m_board);
	m_scene->Add(m_glass);
	m_scene->Add(m_ball);
	m_scene->Add(m_border);
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
	m_ball->PrintPose();
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