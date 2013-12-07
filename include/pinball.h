/*-------------------------------------------------------------------------\
| File: PINBALL.H															|
| Desc: Declarations for a Pinball game (derived from a generic Game class)	|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PINBALL_H
#define PINBALL_H

#include "game.h"
#include "boardObjects.h"
#include "flippers.h"
#include <vector>
#include <string>

using namespace GameFramework;
using namespace Physics;

class Pinball : public Game
{
	private:
		// Actors
		std::vector<Actor*> m_actors;
		
		// Actor Pointers (only kept for actors that need to be accessed after initialization)
		Sphere*		m_ball;
		Plunger*	m_plunger;
		Flippers*	m_flippers;

		/* player's current score */
		int m_currentScore;

		/* Balls at the start of each game */
		const int m_ballsPerGame = 3;

		/* Number of balls remaining for this game */
		int m_ballsRemaining;

		/* Initial Actor positions, used to reset game */
		Transform m_ballInitialPos;

		/* Represents the current gameState */
		enum GameState
		{
			Menu,
			InGame
		} gameState;

		// Actor Initialization Functions
		void InitBoard();
		void InitInnerWalls();
		void InitFlippers();
		void InitPlunger();
		void InitBall();
		void InitJoints();
		void InitCornerWedges();

		/* Add actors in actor vector to game scene */
		void AddActors();

		/* Calculates y offset dependent on board rotation */
		Fl32 calcYOffset(Fl32 zOffset);

		/* Resets Game */
		void Reset();

	public:
		Pinball(std::string title, int windowWidth, int windowHeight);
		~Pinball();

		// Board Actor
		static Board* board;

		/* Initialization override */
		virtual void Init			  ()									override final;

		/* Callback Override Functions */
		virtual void Render			  ()									override final;
		virtual void Idle			  ()									override final;
		virtual void Reshape		  (int width, int height)				override final;
		virtual void MouseButton	  (int button, int state, int x, int y) override final;
		virtual void MouseMove		  (int x, int y)				        override final;
		virtual void KeyboardDown	  (unsigned char key, int x, int y)	    override final;
		virtual void KeyboardUp		  (unsigned char key, int x, int y)	    override final;
		virtual void SpecKeyboardDown (int key, int x, int y)				override final;
		virtual void SpecKeyboardUp	  (int key, int x, int y)				override final;
		virtual void Exit			  ()									override final;
};

#endif // PINBALL_H