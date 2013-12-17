/*-------------------------------------------------------------------------\
| File: PINBALL.CPP															|
| Desc: Provides implementations for a pinball game.						|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "boardObjects.h"
#include "globals.h"
#include "util.h"

// Assign Constants
const int MAX_NUM_ACTOR_SHAPES = 128;

Board* Pinball::board;

int Pinball::m_currentScore = 0;

Pinball::Pinball(std::string title, int windowWidth, int windowHeight)
	: GLUTGame(title, windowWidth, windowHeight)
{
	m_ball = nullptr;
	m_plunger = nullptr;
	m_flippers = nullptr;
	m_actors = std::vector<Actor*>();
	m_monitor = Monitor();
}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
	// Not Paused
	m_paused = false;

	// Set FPS
	m_fps = 1.f / FPS;

	// Set State
	gameState = GameState::Menu;

	// Initialize Camera
	InitCamera();

	// Set Clear Color
	SetClearColor(GetClearColor()); 

	// Initialize All Actors
	Log::Write("Initializing Game Actors...\n", ENGINE_LOG);
	InitBoard();
	InitInnerWalls();
	InitFlippers();
	InitBall();
	InitPlunger();
	InitCornerWedges();
	InitCenterBumpers();

	// Add Actors in game to scene
	AddActors();

	// Add Joints for Plunger
	InitJoints();

	// Initialize Gameplay data
	m_ballsRemaining = m_ballsPerGame;
	m_currentScore = 0;

	// Initialize HUD
	InitHUD();

	// Initialize Timers
	m_plungerTimer = Timer();
	m_scoreTimer = Timer();
	m_gameDuration = Timer();
}

void Pinball::AddActors()
{
	Log::Write("Adding Actors to scene...\n", ENGINE_LOG);

	for (std::vector<Actor*>::iterator iter = m_actors.begin(); iter != m_actors.end(); iter++)
	{
		Actor* act = *iter;
		m_scene->Add(act);
	}
}

void Pinball::TogglePause()
{
	m_scene->TogglePause();
	m_paused = !m_paused;

	if (m_paused)
		gameState = GameState::Paused;
	else
		gameState = GameState::InGame;

	InitHUD();
}

Fl32 Pinball::calcYOffset(Fl32 zOffset)
{
	return tanf(DEG2RAD(25)) * zOffset;
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gameState == GameState::InGame || gameState == GameState::Paused)
	{
		// Update Camera
		camera.Update();

		// Render Scene
		std::vector<physx::PxRigidActor*> actors = m_scene->GetActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC | physx::PxActorTypeSelectionFlag::eRIGID_STATIC); // get scene actors

		int nbActors = actors.size();
		PxShape* shapes[MAX_NUM_ACTOR_SHAPES]; // Pointer to current actor shapes

		if (actors.size())
		{
			for (int i = 0; i < nbActors; i++)
			{
				if (i != GLASS_ATR_IDX) // Don't Render the glass
				{
					PxU32 nbShapes = actors[i]->getNbShapes();
					if (nbShapes <= MAX_NUM_ACTOR_SHAPES)
					{
						actors[i]->getShapes(shapes, nbShapes);

						bool isSleeping = actors[i]->isRigidDynamic() && actors[i]->isRigidDynamic()->isSleeping(); // Check if actor is sleeping

						for (int j = 0; j < nbShapes; j++)
						{
							Transform p = PxShapeExt::getGlobalPose(*shapes[j], *actors[i]);
							PxGeometryHolder h = shapes[j]->getGeometry();

							Mat44 pose(p); // Create Matrix from vector

							glPushMatrix();
							glMultMatrixf((Fl32*)&pose); // Multiply current matrix by pose

							Vec3 aColor = *((Vec3*)actors[i]->userData);
							glColor3f(aColor.x, aColor.y, aColor.z);

							if (h.getType() == PxGeometryType::ePLANE)
								glDisable(GL_LIGHTING);

							GLUTGame::RenderGeometry(h);

							if (h.getType() == PxGeometryType::ePLANE)
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

		switch (gameState)
		{
		case GameState::InGame:
			m_scene->UpdatePhys(m_fps);
			break;
		}
	}

	hud.Render(camera.FOV);
	glutSwapBuffers();
	GLUTGame::Render();
}

void Pinball::Idle()
{
	GLUTGame::Idle();

	/* Check if plunger needs reset */
	if (m_plunger->IsReady() == false)
	{
		m_plungerTimer.Update(deltaTime);

		if (m_plungerTimer.Seconds() > 1)
		{
			m_plunger->Reset();
			m_plungerTimer.Reset();
		}
	}

	/* Check if score needs adjusting */
	if (m_ballInPlay)
	{
		m_gameDuration.Update(deltaTime);
		m_scoreTimer.Update(deltaTime);

		if (m_scoreTimer.Seconds() >= 1)
		{
			m_currentScore += m_scorePerSecond;
			m_scoreTimer.Reset();
			hud.UpdateItem("Score", m_currentScore);
		}
	}
	else if (m_ball->Pose().p.x > 0.4)// Else, check if ball needs to be set to in play
		m_ballInPlay = true;

	/* Check if board is on table, if not adjust accordingly */
	if (m_ball->Get().dynamicActor->getGlobalPose().p.z < -3)
	{
		m_monitor.AddBall(m_currentScore, m_gameDuration.Seconds());
		m_ballsRemaining--;
		m_ballInPlay = false;
		m_ball->Get().dynamicActor->setGlobalPose(m_ballInitialPos);
		hud.UpdateItem("Balls Left", m_ballsRemaining);

		if (m_ballsRemaining < 0)
		{
			gameState = GameState::GameOver;
			InitHUD();
			m_monitor.OutputData();
		}
	}

	m_flippers->UpdateTimers(deltaTime);
}

void Pinball::Reshape(int width, int height)
{
	GLUTGame::Reshape(width, height);
}

void Pinball::MouseButton(int button, int state, int x, int y)
{

}

void Pinball::MouseMove(int x, int y)
{

}

void Pinball::KeyboardDown(unsigned char key, int x, int y)
{
	switch (gameState)
	{
	/* Menu Keys */
	case GameState::Menu:
		if (key == VK_RETURN)
		{
			gameState = GameState::InGame;
			Reset();
			InitHUD();
			glutPostRedisplay();
		}
		break;
	/* InGame Keys */
	case GameState::InGame:
		if (key == VK_SPACE)
		{
			if (m_plunger->IsReady())
				m_plunger->SetKinematicTarget(Transform(Vec3(0, 0, -.02f)));
		}
		if (key == VK_RETURN)
			m_plunger->Reset();
		break;
	/* Game Over Keys */
	case GameState::GameOver:
		if (key == VK_RETURN)
		{
			gameState = GameState::Menu;
			InitHUD();
		}
		break;
	}

	if (gameState == GameState::InGame || gameState == GameState::Paused)
	{
		if (key == 'p')
			TogglePause();
	}

	// Keys applicable to all states
	if(key == VK_ESCAPE)
		exit(0);
}

void Pinball::KeyboardUp(unsigned char key, int x, int y)
{
	switch(gameState)
	{
	case GameState::InGame:
		if (key == VK_SPACE)
		{
			m_plunger->SetKinematic(false);
			m_plunger->SetReady(false);
		}
	}
}

void Pinball::SpecKeyboardDown(int key, int x, int y)
{
	switch (gameState)
	{
	case GameState::Menu:
		break;
	case GameState::InGame:
		if (key == GLUT_KEY_LEFT)
			m_flippers->FlipLeft();
		if (key == GLUT_KEY_RIGHT)
			m_flippers->FlipRight();
		break;
	}
}

void Pinball::SpecKeyboardUp(int key, int x, int y)
{

}

void Pinball::Reset()
{
	m_currentScore = 0;
	m_ballsRemaining = m_ballsPerGame;
	m_ball->Get().dynamicActor->setGlobalPose(m_ballInitialPos);
	m_plunger->Reset();
	m_gameDuration.Reset();
	m_ballInPlay = false;
}

void Pinball::AddScore(int score)
{
	m_currentScore += score;
}

void Pinball::Exit()
{
	GLUTGame::Exit();

	RELEASE(m_ball);
	RELEASE(board);
}