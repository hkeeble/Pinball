/*-------------------------------------------------------------------------\
| File: PINBALL.H															|
| Desc: Declarations for a Pinball game (derived from a generic Game class)	|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#ifndef PINBALL_H
#define PINBALL_H

// STL Includes
#include <vector>
#include <string>

// Internal Includes
#include "glutGame\glutGame.h"
#include "boardObjects.h"
#include "flippers.h"
#include "spinners.h"
#include "glutGame\hud.h"
#include "timer.h"
#include "monitor.h"
#include "image.h"
#include "materialCollection.h"

// Using framework and physics namespaces
using namespace GameFramework;
using namespace Physics;

// Simulation callback for scoring
class ScoreCallback : public Physics::SimulationEventCallback
{
public:
	ScoreCallback();
	virtual ~ScoreCallback();

	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);

	virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) {}
	virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
	virtual void onWake(PxActor **actors, PxU32 count) {}
	virtual void onSleep(PxActor **actors, PxU32 count) {}
};

class Pinball : public GLUTGame
{
	private:
		/* Contants */
		const int m_ballsPerGame = 1;
		const int m_scorePerHighBumper = 100;
		const int m_scorePerLowBumper = 50;
		const int m_bumperBounceMultiplier = 50;

		/* Collection of materials, for central editting file */
		const MaterialCollection m_materials = MaterialCollection();

		/* Switch colors */
		const Vec3 m_switchOnColor = Vec3(1.f, .0f, .0f);
		const Vec3 m_switchOffColor = Vec3(0.f, 1.f, 0.f);

		/* Actors */
		std::vector<Actor*> m_actors;
		
		/* Menu Images */
		Image titleImg, gameOverImg, aboutImg, instructionImg, instruction2Img, backgroundImg;

		/* Actor Pointers (only kept for actors that need to be accessed after initialization) */
		Sphere*		m_ball;
		Plunger*	m_plunger;
		Flippers*	m_flippers;
		Spinners*	m_spinners;
		Box			*m_spinnerSwitchLft, *m_spinnerSwitchRgt;

		/* player's current score */
		int m_currentScore;

		/* Number of balls remaining for this game */
		int m_ballsRemaining;

		/* Initial Actor positions, used to reset game */
		Transform m_ballInitialPos;

		/* Represents the current gameState */
		enum GameState
		{
			Menu,
			Instructions,
			Instructions2,
			About,
			InGame,
			GameOver,
			Paused
		} gameState;

		/* Camera Initialization Functions */
		void Init3DCamera();
		void Init2DCamera();

		/* Actor Initialization Functions */
		void InitBoard();
		void InitInnerWalls();
		void InitFlippers();
		void InitPlunger();
		void InitBall();
		void InitJoints();
		void InitCornerWedges();
		void InitStartingBlocks();
		void InitHighBumpers();
		void InitLowBumpers();
		void InitSpinners();
		void InitSpinnerSwitches();

		/* HUD Initialization Function */
		void InitHUD();

		/* Add actors in actor vector to game scene */
		void AddActors();

		/* Is Paused */
		bool m_paused;

		/* Toggles Pause */
		void TogglePause();

		/* Creates a board position transform */
		Transform CreatePosition(const Fl32& x, const Fl32& z);

		/* Calculates y offset dependent on board rotation */
		Fl32 calcYOffset(Fl32 zOffset);

		/* Resets Game */
		void Reset();

		/* Represents the game's HUD */
		HUD hud;

		/* Used to Time the plunger */
		Timer m_plungerTimer;

		/* Used to track time for scoring */
		Timer m_scoreTimer;

		/* Used to track game duration */
		Timer m_gameDuration;

		/* Used to determine if ball is in play, and scoring should be happening */
		bool m_ballInPlay;

		/* Score gained per second if ball is in play */
		const int m_scorePerSecond = 5;

		/* Used to monitor in game data */
		Monitor m_monitor;

		/* Used for monitoring individual ball data */
		int m_scoreForThisBall;
		Timer m_durationThisBallInPlay;

	public:
		/* Construction and Destruction */
		Pinball(std::string title, int windowWidth, int windowHeight);
		~Pinball();

		/* Board Actor */
		static Board* board;

		/* Activates spinners if they are inactive */
		void ActivateSpinners();

		/* Used by triggers to update the game */
		static bool AddScoreHigh;
		static bool AddScoreLow;
		static bool BounceBall;
		static bool EnableSpinners;
		static Vec3 BallBounceDirection;

		/* Initialization override */
		virtual void Init			  ()									override final;

		/* Game Scene Initialization */
		virtual void InitGame();

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