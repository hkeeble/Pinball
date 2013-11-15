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
		Sphere* m_ball;
		Box* m_board;
		Box* m_glass;
		Plane* m_ground;
		Border* m_border; 

		// Constants
		const Fl32 m_ballRadius; // 1/16th of an inch

		// Board Height and Width
		const Vec3 m_boardDimensions;
		const Transform m_boardPose;
		const Fl32 m_fallHoleWidth;
		const Fl32 m_plungerLaneWidth;

	public:
		Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY);
		~Pinball();

		Vec3 Dimensions() const;
		Transform Pose() const;
		Fl32 FallHoleWidth() const;
		Fl32 PlungerLaneWidth() const;
		Fl32 BallRadius() const;

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