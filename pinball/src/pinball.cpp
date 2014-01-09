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

bool Pinball::AddScoreHigh = false;
bool Pinball::AddScoreLow = false;
bool Pinball::BounceBall = false;
bool Pinball::EnableSpinners = false;
Vec3 Pinball::BallBounceDirection = Vec3(0);

Pinball::Pinball(std::string title, int windowWidth, int windowHeight)
	: GLUTGame(title, windowWidth, windowHeight)
{
	m_ball = nullptr;
	m_plunger = nullptr;
	m_flippers = nullptr;
	m_actors = std::vector<Actor*>();
	m_monitor = Monitor();
	m_currentScore = 0;
	m_scoreForThisBall = 0;
}

Pinball::~Pinball()
{

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

Transform Pinball::CreatePosition(const Fl32& x, const Fl32& z)
{
	return Transform(Vec3(x, calcYOffset(z), z));
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gameState == GameState::InGame || gameState == GameState::Paused)
	{
		Init2DCamera();
		backgroundImg.Render();
		Init3DCamera();

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

							if (i == 0) // Board is textured
								GLUTGame::RenderGeometry(h, true);
							else
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

		/* Update Physics */
		m_scene->UpdatePhys(1 / FPS);

		CalculateFrameRate();
		hud.UpdateItem("FPS", m_fps);
	}
	if (gameState == GameState::Menu)
		titleImg.Render();
	else if (gameState == GameState::Instructions)
		instructionImg.Render();
	else if (gameState == GameState::Instructions2)
		instruction2Img.Render();
	else if (gameState == GameState::About)
		aboutImg.Render();
	else if (gameState == GameState::GameOver)
	{
		gameOverImg.Render();
		hud.Render(camera.FOV);
	}
	else
		hud.Render(camera.FOV);

	glutSwapBuffers();
}

void Pinball::Idle()
{
	GLUTGame::Idle();

	if (gameState == GameState::InGame)
	{
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
			m_durationThisBallInPlay.Update(deltaTime);
			m_scoreTimer.Update(deltaTime);

			if (m_scoreTimer.Seconds() >= 1)
			{
				m_currentScore += m_scorePerSecond;
				m_scoreForThisBall += m_scorePerSecond;
				m_scoreTimer.Reset();
				hud.UpdateItem("Score", m_currentScore);
			}
		}
		else if (m_ball->Pose().p.x > 0.4)// Else, check if ball needs to be set to in play
			m_ballInPlay = true;

		/* Check if ball is on table, if not adjust accordingly */
		if (m_ball->Get().dynamicActor->getGlobalPose().p.y < -3)
		{
			m_monitor.AddBall(m_scoreForThisBall, m_durationThisBallInPlay.Seconds());

			m_durationThisBallInPlay.Reset();
			m_scoreForThisBall = 0;

			m_ballsRemaining--;
			m_ballInPlay = false;
			m_ball->Get().dynamicActor->setLinearVelocity(Vec3(0));
			m_ball->Get().dynamicActor->setAngularVelocity(Vec3(0));
			m_ball->Get().dynamicActor->setGlobalPose(m_ballInitialPos);
			hud.UpdateItem("Balls Left", m_ballsRemaining);

			if (m_ballsRemaining == 0)
			{
				gameState = GameState::GameOver;
				InitHUD();
				m_monitor.OutputData();
			}
		}

		/* Check for bumper scoring */
		if (AddScoreHigh)
		{
			m_currentScore += m_scorePerHighBumper;
			m_scoreForThisBall += m_scorePerHighBumper;
			AddScoreHigh = false;
		}

		if (AddScoreLow)
		{
			m_currentScore += m_scorePerLowBumper;
			m_scoreForThisBall += m_scorePerLowBumper;
			AddScoreLow = false;
		}

		/* Check for bounces */
		if (BounceBall)
		{
			m_ball->Get().dynamicActor->addForce(BallBounceDirection*m_bumperBounceMultiplier);
			BounceBall = false;
		}

		/* Check if spinners need activating */
		if (EnableSpinners)
		{
			ActivateSpinners();
			EnableSpinners = false;
		}

		/* Update Spinners */
		m_spinners->Update(deltaTime);

		if (m_spinners->Active() == false)
		{
			m_spinnerSwitchLft->Get().dynamicActor->userData = &const_cast<Vec3&>(m_switchOffColor);
			m_spinnerSwitchRgt->Get().dynamicActor->userData = &const_cast<Vec3&>(m_switchOffColor);
		}
	}
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
			Init3DCamera();
			glutPostRedisplay();
		}
		if (key == 'i')
		{
			gameState = GameState::Instructions;
			glutPostRedisplay();
		}
		if (key == 'a')
		{
			gameState = GameState::About;
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
		{
			if (m_flippers)
				m_flippers->Flip();
		}
		if (key == 's')
		{
			m_spinners->Toggle();
		}
		break;
		/* Game Over Keys */
	case GameState::GameOver:
		if (key == VK_RETURN)
		{
			gameState = GameState::Menu;
			Reset();
			InitHUD();
		}
		break;
	}

	if (gameState == GameState::About || gameState == GameState::Instructions2)
	{
		if (key == VK_RETURN)
		{
			gameState = GameState::Menu;
			glutPostRedisplay();
		}
	}

	if (gameState == GameState::Instructions)
	{
		if (key == VK_RETURN)
		{
			gameState = GameState::Instructions2;
			glutPostRedisplay();
		}
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
		if (key == VK_RETURN)
		{
			if (m_flippers)
				m_flippers->Unflip();
		}
	}
}

void Pinball::SpecKeyboardDown(int key, int x, int y)
{

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
	if (m_spinners->Active())
		m_spinners->Toggle();
	m_monitor.Clear();
}

void Pinball::ActivateSpinners()
{
	if (m_spinners->Active() == false)
	{
		m_spinners->Toggle();
		m_spinnerSwitchLft->Get().dynamicActor->userData = &const_cast<Vec3&>(m_switchOnColor);
		m_spinnerSwitchRgt->Get().dynamicActor->userData = &const_cast<Vec3&>(m_switchOnColor);
	}
}

void Pinball::Exit()
{
	GLUTGame::Exit();

	RELEASE(m_ball);
	RELEASE(board);
}