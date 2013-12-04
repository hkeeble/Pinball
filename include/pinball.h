/*-------------------------------------------------------------------------\
| File: PINBALL.H															|
| Desc: Declarations for a Pinball game (derived from a generic Game class)	|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PINBALL_H
#define PINBALL_H

#include "game.h"
#include "boardObjects.h"
#include <string>

using namespace GameFramework;
using namespace Physics;

class Pinball : public Game
{
	private:
		// Actors
		Sphere*		m_ball;
		Box*		m_glass;
		Border*		m_border;
		InnerWalls* m_innerWalls;
		Plunger*	m_plunger;
		Flipper*	m_rgtFlipper;
		Flipper*	m_lftFlipper;

		int m_currentScore;

		enum GameState
		{
			Menu,
			InGame
		} gameState;

	public:
		Pinball(std::string title, int windowWidth, int windowHeight);
		~Pinball();

		// Board Actor
		static Board* board;

		void InitJoints();

		virtual void Init();
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
};

#endif // PINBALL_H