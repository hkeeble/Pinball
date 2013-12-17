/*-------------------------------------------------------------------------\
| File: GAME.CPP															|
| Desc: Provides implementations for a generic Game object with PhysX		|
|		physics simulation, using GLUT for input and window management.		|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "glutGame.h"
#include <iostream>

using namespace physx;

namespace GameFramework
{
	GLUTGame* GLUTGame::instance = NULL;

	Vec3 GLUTGame::ClearColor = Vec3(.5f, 1.f, 1.f);

	static const int MAX_NUM_CONVEXMESH_TRIANGLES = 1024;
	static unsigned int gConvexMeshTriIndices[3 * MAX_NUM_CONVEXMESH_TRIANGLES];

	GLUTGame::GLUTGame(std::string title, int windowWidth, int windowHeight)
	{
		Physics::PxInit(); // initialize physics

		m_scene = new Physics::Scene();
		m_scene->Init(); // initialize scene

		instance = this; // Assign current instance for callback wrapper functions

		m_title = title;
		m_windowHeight = windowHeight;
		m_windowWidth = windowWidth;

		lastElapsedTime = 0;
		newElapsedTime = 0;
		deltaTime = 0;
	}

	GLUTGame::~GLUTGame()
	{
		RELEASE(m_scene);
	}

	void GLUTGame::Run(int argc, char *argv[])
	{
		Log::Write("Game Run Function Invoked...\n", ENGINE_LOG);
		
		InitGLUT(argc, argv);
		InitGL();
		
		Log::Write("Initializing Game...\n", ENGINE_LOG);
		Init();

		Log::Write("Entering main game loop...\n", ENGINE_LOG);
		glutMainLoop();
	}

	void GLUTGame::TimerFunc(int id)
	{
		glutPostRedisplay();
	}

	void GLUTGame::Init()
	{

	}

	void GLUTGame::UpdatePerspective(const Fl32& FOV)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		Fl32 r = glutGet(GLUT_INIT_WINDOW_WIDTH) / glutGet(GLUT_INIT_WINDOW_HEIGHT);
		gluPerspective(FOV, r, .1, 100);
		glMatrixMode(GL_MODELVIEW);
	}

	void GLUTGame::InitGL() // Virtual Initialization - Override to change lighting params
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

	void GLUTGame::InitGLUT(int argc, char *argv[])
	{
		Log::Write("Intializing GLUT...\n", ENGINE_LOG);

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
		glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-m_windowWidth)/2,
							   (glutGet(GLUT_SCREEN_HEIGHT)-m_windowHeight)/2);
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

	void GLUTGame::RenderGeometry(PxGeometryHolder h)
	{
		switch (h.getType())
		{
		case PxGeometryType::eBOX:
			glScalef(h.box().halfExtents.x, h.box().halfExtents.y, h.box().halfExtents.z);
			glutSolidCube(2.0f);
			break;
		case PxGeometryType::eSPHERE:
			glutSolidSphere(h.sphere().radius, RENDER_DETAIL, RENDER_DETAIL);
			break;
		case PxGeometryType::eCONVEXMESH:
			PxConvexMesh* mesh = h.convexMesh().convexMesh;
			const PxU32 nbPolys = mesh->getNbPolygons();
			const PxU8* polygons = mesh->getIndexBuffer();
			const PxVec3* verts = mesh->getVertices();
			PxU32 numTotalTriangles = 0;

			for (PxU32 i = 0; i < nbPolys; i++)
			{
				PxHullPolygon data;
				mesh->getPolygonData(i, data);

				const PxU32 nbTris = data.mNbVerts - 2;
				const PxU8 vref0 = polygons[data.mIndexBase + 0];
				for (PxU32 j = 0; j < nbTris; j++)
				{
					const PxU32 vref1 = polygons[data.mIndexBase + 0 + j + 1];
					const PxU32 vref2 = polygons[data.mIndexBase + 0 + j + 2];
					if (numTotalTriangles < MAX_NUM_CONVEXMESH_TRIANGLES)
					{
						gConvexMeshTriIndices[3 * numTotalTriangles + 0] = vref0;
						gConvexMeshTriIndices[3 * numTotalTriangles + 1] = vref1;
						gConvexMeshTriIndices[3 * numTotalTriangles + 2] = vref2;
						numTotalTriangles++;
					}
				}
			}

			if (numTotalTriangles < MAX_NUM_CONVEXMESH_TRIANGLES)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, verts);
				glDrawElements(GL_TRIANGLES, numTotalTriangles * 3, GL_UNSIGNED_INT, gConvexMeshTriIndices);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
		}
	}

	void GLUTGame::SetClearColor(const Vec3& color)
	{
		ClearColor = color;
		glClearColor(color.x, color.y, color.z, 1.f);
	}

	Vec3 GLUTGame::GetClearColor()
	{
		return ClearColor;
	}

	/*-------------------------------------------------------------------------\
	|				VIRTUAL CALLBACK FUNCTION DEFINITIONS						|
	\-------------------------------------------------------------------------*/

	void GLUTGame::Render()
	{
		glutTimerFunc(16.6, TimerFuncWrapper, 0);
	}

	void GLUTGame::Idle()
	{
		newElapsedTime = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = newElapsedTime - lastElapsedTime;
		lastElapsedTime = newElapsedTime;
	}

	void GLUTGame::Reshape(int width, int height)
	{
		glViewport(0, 0, width, height);
		UpdatePerspective(camera.FOV);
	}

	void GLUTGame::MouseButton(int button, int state, int x, int y)
	{

	}

	void GLUTGame::MouseMove(int x, int y)
	{

	}

	void GLUTGame::KeyboardDown(unsigned char key, int x, int y)
	{

	}

	void GLUTGame::KeyboardUp(unsigned char key, int x, int y)
	{

	}

	void GLUTGame::SpecKeyboardDown(int key, int x, int y)
	{

	}

	void GLUTGame::SpecKeyboardUp(int key, int x, int y)
	{

	}

	void GLUTGame::Exit()
	{
		Log::Shutdown();
	}

	/*-------------------------------------------------------------------------\
	| WRAPPER FUNCTION DEFINITIONS - USED TO CALL INSTANCE EQUIVALENT FUNCTIONS |
	\-------------------------------------------------------------------------*/

	void GLUTGame::RenderWrapper() { instance->Render(); }
	void GLUTGame::IdleWrapper() { instance->Idle(); }
	void GLUTGame::ReshapeWrapper(int width, int height) { instance->Reshape(width, height); }

	// Mouse Wrappers
	void GLUTGame::MouseButtonWrapper(int button, int state, int x, int y) { instance->MouseButton(button, state, x, y); }
	void GLUTGame::MouseMoveWrapper(int x, int y) { instance->MouseMove(x, y); }

	// Keyboard Wrappers
	void GLUTGame::KeyboardDownWrapper(unsigned char key, int x, int y) { instance->KeyboardDown(key, x, y); }
	void GLUTGame::KeyboardUpWrapper(unsigned char key, int x, int y) { instance->KeyboardUp(key, x, y); }
	void GLUTGame::SpecKeyboardDownWrapper(int key, int x, int y) { instance->SpecKeyboardDown(key, x, y); }
	void GLUTGame::SpecKeyboardUpWrapper(int key, int x, int y) { instance->SpecKeyboardUp(key, x, y); }

	// Exit Wrapper
	void GLUTGame::ExitWrapper() { instance->Exit(); }

	// Timer Wrapper
	void GLUTGame::TimerFuncWrapper(int id) { instance->TimerFunc(id); }
}