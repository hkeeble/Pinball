/*-------------------------------------------------------------------------\
| File: GAME.CPP															|
| Desc: Provides declarations for a generic Game object with PhysX			|
|		physics simulation, using GLUT for input and window management.		|
| Definition File: GAME.CPP													|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef GAME_H
#define GAME_H

#include <string>
#include <time.h>
#include "camera.h"
#include "gameState.h"
#include "Physics.h"
#include "Actors.h"
#include "glut.h"
#include "uncopyable.h"
#include "log.h"
#include "timer.h"
#include "BASS\bass.h"

#define FPS 60.f
#define RENDER_DETAIL 15

namespace GameFramework
{
	class GLUTGame : private Uncopyable
	{
	private:
		// Pointer to current instance (for callback wrappers)
		static GLUTGame* instance;

		// Window Parameters
		std::string m_title;
		int m_windowHeight;
		int m_windowWidth;

		// GLUT Initialization
		void InitGLUT(int argc, char *argv[]);

		/* BASS Initialization */
		void InitBASS();

		/* The game's clear color */
		static Vec3 ClearColor;

		/* Used For timers */
		clock_t lastElapsedTime;
		clock_t newElapsedTime;

		/* Renders a textured cube */
		void RenderTexturedCube(GLint size);

		/* Milliseconds since the last frame */
		Fl32 m_milliSecondsSinceLastFrame;

		/* Calculates Delta Time */
		void CalculateDeltaTime();

	protected:
		// GL Initialization
		virtual void InitGL();

		// Physics Simulation Objects
		Physics::Scene* m_scene;
		Fl32 m_pxTimeStep;

		// Renders the given geometric object
		void RenderGeometry(physx::PxGeometryHolder h, bool textured = false);

		Camera camera;

		/* Current Delta Time */
		Fl32 deltaTime;
		
		/* Physics Time Step Timer */
		Fl32 simTimer;

		/* Get window dimensions */
		Vec2 WindowDimensions() const;
		
		/* Is using a 2D camera */
		bool m_is2D;

		/* Used for frame rate calculations */
		int m_fps, m_time, m_timebase, m_frame;
		void CalculateFrameRate();

	public:
		// Construction/Destruction
		GLUTGame(std::string title, int windowWidth, int windowHeight);
		~GLUTGame();

		/* Updates the games projection matrix */
		static void UpdatePerspective(const Fl32& FOV);

		/* Set the game's clear color */
		void SetClearColor(const Vec3& color);

		/* Get the game's clear color */
		static Vec3 GetClearColor();

		void Run(int argc, char *argv[]); // Enters the main loop

		virtual void Init();

		// Instance Callback Functions
		virtual void Render();
		virtual void Idle();
		virtual void Reshape(int width, int height);
		virtual void MouseButton(int button, int state, int x, int y) = 0;
		virtual void MouseMove(int x, int y) = 0;
		virtual void KeyboardDown(unsigned char key, int x, int y) = 0;
		virtual void KeyboardUp(unsigned char key, int x, int y) = 0;
		virtual void SpecKeyboardDown(int key, int x, int y) = 0;
		virtual void SpecKeyboardUp(int key, int x, int y) = 0;
		virtual void Exit();

		// Callback wrappers, used to call instance callbacks
		static void RenderWrapper();
		static void IdleWrapper();
		static void ReshapeWrapper(int width, int height);
		static void MouseButtonWrapper(int button, int state, int x, int y);
		static void MouseMoveWrapper(int x, int y);
		static void KeyboardDownWrapper(unsigned char key, int x, int y);
		static void KeyboardUpWrapper(unsigned char key, int x, int y);
		static void SpecKeyboardDownWrapper(int key, int x, int y);
		static void SpecKeyboardUpWrapper(int key, int x, int y);
		static void ExitWrapper();
	};
}

#endif #GAME_H