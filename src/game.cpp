/*-------------------------------------------------------------------------\
| File: GAME.CPP															|
| Desc: Provides implementations for a generic Game object with PhysX		|
|		physics simulation, using GLUT for input and window management.		|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "game.h"
#include <iostream>

using namespace physx;

namespace GameFramework
{
	Game* Game::instance = NULL;

	static Fl32 gPlaneData[]={
			-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};

	Game::Game(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY)
	{
		Physics::PxInit(); // initialize physics

		m_scene = new Physics::Scene();
		m_scene->Init(); // initialize scene

		instance = this; // Assign current instance for callback wrapper functions

		m_title = title;
		m_windowHeight = windowHeight;
		m_windowWidth = windowWidth;
		m_windowPosX = windowPosX;
		m_winowPosY = windowPosY;

		m_fps = 1.f/FPS;
	}

	Game::~Game()
	{
		if(m_scene)
			delete m_scene;
	}

	void Game::Run(int argc, char *argv[])
	{
		Log::Write("Game Run Function Invoked...\n", ENGINE_LOG);
		
		InitGLUT(argc, argv);
		InitGL();
		
		Log::Write("Initializing Game...\n", ENGINE_LOG);
		Init();

		Log::Write("Entering main game loop...\n", ENGINE_LOG);
		glutMainLoop();
	}

	void Game::Init()
	{

	}

	void Game::InitGL() // Virtual Initialization - Override to change lighting params
	{
		Log::Write("Intializing OpenGL...\n", ENGINE_LOG);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		Fl32 ambientColor[]	= { .5f, .5f,  .5f, 1.f };
		Fl32 diffuseColor[]	= { .4f, .4f,  .4f, 1.f };		
		Fl32 specularColor[]= { 1.f, 1.f,  1.f, 1.f };		
		Fl32 position[]		= { 1.f, 0.f, 0.f, 0.0f };		
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);

	}

	void Game::InitGLUT(int argc, char *argv[])
	{
		Log::Write("Intializing GLUT...\n", ENGINE_LOG);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
		glutInitWindowPosition(m_windowPosX, m_winowPosY);
		glutInitWindowSize(m_windowWidth, m_windowHeight);
		glutCreateWindow(m_title.c_str());

		// Assign wrappers to callbacks
		glutDisplayFunc(RenderWrapper);
		glutIdleFunc(IdleWrapper);
		glutReshapeFunc(ReshapeWrapper);
		glutMotionFunc(MouseMoveWrapper);
		glutMouseFunc(MouseButtonWrapper);
		glutKeyboardFunc(KeyboardDownWrapper);
		glutKeyboardUpFunc(KeyboardUpWrapper);
		glutSpecialFunc(SpecKeyboardDownWrapper);
		glutSpecialUpFunc(SpecKeyboardUpWrapper);
		atexit(ExitWrapper);
	}

	void Game::RenderGeometry(PxGeometryHolder h)
	{
		switch(h.getType())
		{
		case PxGeometryType::eBOX:
			glScalef(h.box().halfExtents.x, h.box().halfExtents.y, h.box().halfExtents.z);
			glutSolidCube(2.0f);
			break;
		case PxGeometryType::eSPHERE:		
			glutSolidSphere(h.sphere().radius, RENDER_DETAIL, RENDER_DETAIL);
			break;
		case PxGeometryType::ePLANE:
			glScalef(10240,0,10240);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			glVertexPointer(3, GL_FLOAT, 2*3*sizeof(Fl32), gPlaneData);
			glNormalPointer(GL_FLOAT, 2*3*sizeof(Fl32), gPlaneData+3);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}
	}

	/*-------------------------------------------------------------------------\
	|				VIRTUAL CALLBACK FUNCTION DEFINITIONS						|
	\-------------------------------------------------------------------------*/

	void Game::Render()
	{

	}

	void Game::Idle()
	{
		m_scene->Update(m_fps);
		glutPostRedisplay();
	}

	void Game::Reshape(int width, int height)
	{
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH)/glutGet(GLUT_INIT_WINDOW_HEIGHT);
		gluPerspective(camera.FOV, r, .1, 100);
		glMatrixMode(GL_MODELVIEW);
	}

	void Game::MouseButton(int button, int state, int x, int y)
	{

	}

	void Game::MouseMove(int x, int y)
	{

	}

	void Game::KeyboardDown(unsigned char key, int x, int y)
	{

	}

	void Game::KeyboardUp(unsigned char key, int x, int y)
	{

	}

	void Game::SpecKeyboardDown(int key, int x, int y)
	{

	}

	void Game::SpecKeyboardUp(int key, int x, int y)
	{

	}

	void Game::Exit()
	{
		Log::Shutdown();
	}

	/*-------------------------------------------------------------------------\
	| WRAPPER FUNCTION DEFINITIONS - USED TO CALL INSTANCE EQUIVALENT FUNCTIONS |
	\-------------------------------------------------------------------------*/

	void Game::RenderWrapper() { instance->Render(); }
	void Game::IdleWrapper() { instance->Idle(); }
	void Game::ReshapeWrapper(int width, int height) { instance->Reshape(width, height); }

	// Mouse Wrappers
	void Game::MouseButtonWrapper(int button, int state, int x, int y) { instance->MouseButton(button, state, x, y); }
	void Game::MouseMoveWrapper(int x, int y) { instance->MouseMove(x, y); }

	// Keyboard Wrappers
	void Game::KeyboardDownWrapper(unsigned char key, int x, int y) { instance->KeyboardDown(key, x, y); }
	void Game::KeyboardUpWrapper(unsigned char key, int x, int y) { instance->KeyboardUp(key, x, y); }
	void Game::SpecKeyboardDownWrapper(int key, int x, int y) { instance->SpecKeyboardDown(key, x, y); }
	void Game::SpecKeyboardUpWrapper(int key, int x, int y) { instance->SpecKeyboardUp(key, x, y); }

	// Exit Wrapper
	void Game::ExitWrapper() { instance->Exit(); }
}