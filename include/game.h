/*-------------------------------------------------------------------------\
| File: GAME.CPP															|
| Desc: Provides declarations for a generic Game object with PhysX			|
|		physics simulation, using GLUT for input and window management.		|
| Definition File: GAME.CPP													|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef GAME_H
#define GAME_H

#include "Physics.h"
#include "GL/glut.h"
#include "uncopyable.h"
#include <string>

namespace GameFramework
{
	class Game : private Uncopyable
	{
	private:
		// Pointer to current instance (for callback wrappers)
		static Game* instance;

		// Window Parameters
		std::string m_title;
		int m_windowHeight;
		int m_windowWidth;
		int m_windowPosX;
		int m_winowPosY;

		// Physics Simulation Objects
		Physics::Scene m_scene;
		physx::PxReal m_deltaTime;

		// GLUT Initialization
		void InitGLUT(int argc, char *argv[]);

	protected:
		// GL Initialization
		virtual void InitGL();

	public:
		// Construction/Destruction
		Game(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY, physx::PxReal deltaTime = 1.f/60.f);
		~Game();

		void Run(int argc, char *argv[]); // Enters the main loop

		// Instance Callback Functions
		virtual void Render();
		virtual void Idle();
		virtual void Reshape(int width, int height);
		virtual void MouseButton(int button, int state, int x, int y);
		virtual void MouseMove(int x, int y);
		virtual void KeyboardDown(unsigned char key, int x, int y);
		virtual void KeyboardUp(unsigned char key, int x, int y);
		virtual void SpecKeyboardDown(int key, int x, int y);
		virtual void SpecKeyboardUp(int key, int x, int y);
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