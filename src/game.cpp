/*-------------------------------------------------------------------------\
| File: GAME.CPP															|
| Desc: Provides implementations for a generic Game object with PhysX		|
|		physics simulation, using GLUT for input and window management.		|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "game.h"
#include <iostream>

namespace GameFramework
{
	Game* Game::instance = NULL;

	Game::Game(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY, physx::PxReal deltaTime)
	{
		Log::SetWorkingDir(GetCurrentDir());
		Log::Write("Initializing Game Instance...\n", ENGINE_LOG);

		Physics::PxInit(); // initialize physics

		m_scene.Init(); // initialize scene

		instance = this; // Assign current instance for callback wrapper functions

		m_title = title;
		m_windowHeight = windowHeight;
		m_windowWidth = windowWidth;
		m_windowPosX = windowPosX;
		m_winowPosY = windowPosY;

		m_deltaTime = deltaTime;
	}

	Game::~Game()
	{

	}

	void Game::Run(int argc, char *argv[])
	{
		Log::Write("Game Run Function Invoked...\n", ENGINE_LOG);

		InitGL();
		InitGLUT(argc, argv);

		Log::Write("Entering main game loop...\n", ENGINE_LOG);
		glutMainLoop();
	}

	void Game::InitGL() // Virtual Initialization - Override to change lighting params
	{
		Log::Write("Intializing OpenGL...\n", ENGINE_LOG);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		float ambientColor[]	= { .5f, .5f, .5f, 1.f };
		float diffuseColor[]	= { 1.f, 1.f, 1.f, 1.f };		
		float specularColor[]	= { 1.f, 1.f, 1.f, 1.f };		
		float position[]		= { 100.f, 100.f, 200.f, 1.0f };		
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
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
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
		glutSpecialFunc(SpecKeyboardUpWrapper);
		atexit(ExitWrapper);
	}

	/*-------------------------------------------------------------------------\
	|				VIRTUAL CALLBACK FUNCTION DEFINITIONS						|
	\-------------------------------------------------------------------------*/

	void Game::Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Scene
		std::vector<physx::PxRigidActor*> actors = m_scene.GetActors(); // get scene actors

		glutSwapBuffers();
	}

	void Game::Idle()
	{
		m_scene.Update(m_deltaTime);
	}

	void Game::Reshape(int width, int height)
	{
		
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