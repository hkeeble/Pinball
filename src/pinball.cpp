/*-------------------------------------------------------------------------\
| File: PINBALL.CPP															|
| Desc: Provides implementations for a pinball game.						|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "boardObjects.h"
#include "globals.h"

// Assign Constants
const int MAX_NUM_ACTOR_SHAPES = 128;

Board* Pinball::board;

Pinball::Pinball(std::string title, int windowWidth, int windowHeight)
	: Game(title, windowWidth, windowHeight)
{
	m_ball = nullptr;
	m_plunger = nullptr;
	m_flippers = nullptr;
	m_actors = std::vector<Actor*>();
}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
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

void Pinball::InitHUD()
{
	hud.Clear();

	switch (gameState)
	{
	case GameState::Menu:
		SetClearColor(Vec3(0.f, 0.f, 0.f));
		hud.SetRenderColor(Vec3(1.f, 1.f, 1.f));
		hud.AddItem("PINBALL!", Vec2(45, 85), HUDFont::largeFont);
		hud.AddItem("Press enter to begin.", Vec2(40, 75), HUDFont::smallFont);
		hud.AddItem("Henri Keeble - Game Engines - Assessment Item 1", Vec2(20, 10), HUDFont::smallFont);
		break;
	case GameState::InGame:
		SetClearColor(Vec3(0.f, 0.8f, 1.f));
		hud.SetRenderColor(Vec3(1.f, 0.f, 0.f));
		hud.AddItem("Score", Vec2(5, 95), HUDFont::largeFont, true, 0);
		hud.AddItem("Balls Left", Vec2(65, 95), HUDFont::largeFont, true, m_ballsRemaining);
		break;
	case GameState::GameOver:
		SetClearColor(Vec3(0.f, 0.f, 0.f));
		hud.SetRenderColor(Vec3(1.f, 1.f, 1.f));
		hud.AddItem("GAME OVER!", Vec2(40, 85), HUDFont::largeFont);
		hud.AddItem("Score", Vec2(45, 75), HUDFont::smallFont, true, m_currentScore);
		hud.AddItem("Game Duration (Seconds)", Vec2(25, 65), HUDFont::smallFont, true, m_gameDuration.Seconds());
		hud.AddItem("Press Enter to continue.", Vec2(25, 55), HUDFont::smallFont);
		break;
	}
}

void Pinball::InitCamera()
{
	Log::Write("Intializing Camera...\n", ENGINE_LOG);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera = { UP_VECTOR, Vec3(0, 0, 0), Vec3(0, 1, -5.5), DEFAULT_FOV };
	camera.Update(); // Used to initialize camera positions (gluLookAt)
	UpdatePerspective(camera.FOV);
}

void Pinball::InitJoints()
{
	Log::Write("Initializing Plunger Spring...\n", ENGINE_LOG);

	// Plunger Spring
	AddDistanceJoint(m_plunger->Get().dynamicActor, PxTransform::createIdentity(), NULL, m_plunger->Get().dynamicActor->getGlobalPose(),
		PxDistanceJointFlag::eSPRING_ENABLED, 1000.f, 1.f);
	m_plunger->SetKinematic(true);
}

void Pinball::InitBall()
{
	Log::Write("\tInitializing Ball...\n", ENGINE_LOG);

	const Vec3 BallColor = Vec3(.8f, .8f, .8f);
	PxMaterial* BallMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);
	const Fl32 BallDensity = 1.f;
	m_ballInitialPos = board->Pose() * Transform(board->Right().x + (board->WallWidth() * 2) + BALL_RADIUS * 2, board->Dimensions().y * 2 + BALL_RADIUS * 2, 0);
	m_ball = new Sphere(m_ballInitialPos, BALL_RADIUS, BallDensity, BallColor, BallMaterial);
	m_actors.push_back(m_ball);
}

void Pinball::InitBoard()
{
	Log::Write("\tInitializing Board...\n", ENGINE_LOG);

	const Vec3 BoardColor = Vec3(.4f, .4f, .4f);
	const Vec3 BorderColor = Vec3(0.f, 0.f, 0.f);

	PxMaterial* BoardMaterial = PHYSICS->createMaterial(0.f, 0.f, .1f);
	PxMaterial* BorderMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);

	// board
	board = new Board(BoardMaterial, BoardColor);

	// Glass Pose
	Transform GlassPose = board->Pose() * Transform(Vec3(0, board->WallHeight() * 2 + (board->Dimensions().y * 2), 0));

	m_actors.push_back(board);
	m_actors.push_back(new Box(GlassPose, board->Dimensions(), 0, Vec3(0, 0, 0), BoardMaterial, StaticActor));
	m_actors.push_back(new Border(BorderMaterial, BorderColor));
}

void Pinball::InitInnerWalls()
{
	Log::Write("\tInitializing Inner Walls...\n", ENGINE_LOG);

	const Vec3 WallColor = Vec3(0.f, 0.f, 0.f);
	PxMaterial* WallMaterial = PHYSICS->createMaterial(0.f, 0.f, .2f);

	m_actors.push_back(new InnerWalls(WallMaterial, WallColor));
}

void Pinball::InitFlippers()
{
	Log::Write("\tInitializing Flippers...\n", ENGINE_LOG);

	// Flippers Data
	const Vec3 FlipperDimensions = Vec3(board->WallWidth(), board->WallHeight(), 0.1f);
	const Vec3 FlipperColor = Vec3(.5f, 0.f, 0.f);
	const Fl32 FlipperDensity = .5f;
	PxMaterial* FlipperMaterial = PHYSICS->createMaterial(0.f, 0.f, 0.1f);
	
	// Determine Positions
	const Fl32 zOffset = board->Bottom().z + 0.5f;
	const Fl32 xOffset = .2f;
	const Fl32 yOffset = calcYOffset(zOffset);

	// Flipper Positions
	const Transform lftFPos = Transform(Vec3(.26f,  yOffset+FlipperDimensions.y*2, zOffset));
	const Transform rgtFPos = Transform(Vec3(-.12f, yOffset+FlipperDimensions.y*2, zOffset));

	// Create Flippers
	Flipper* m_lftFlipper = new Flipper(lftFPos, FlipperType::Left, FlipperMaterial, FlipperColor, FlipperDensity);
	Flipper* m_rgtFlipper = new Flipper(rgtFPos, FlipperType::Right, FlipperMaterial, FlipperColor, FlipperDensity);

	// Create Flippers Object
	m_flippers = new Flippers(m_lftFlipper, m_rgtFlipper);

	// Add Flippers to Actors Vector
	m_actors.push_back(m_flippers->GetLeft());
	m_actors.push_back(m_flippers->GetRight());
}

void Pinball::InitPlunger()
{
	Log::Write("\tInitializing Plunger...\n", ENGINE_LOG);

	PxMaterial* PlungerMaterial = PHYSICS->createMaterial(0.f, 0.f, .1f);
	const Vec3 PlungerColor = Vec3(.1f, .1f, 1.f);
	const Fl32 PlungerDensity = 2.f;

	m_plunger = new Plunger(PlungerMaterial, PlungerColor, PlungerDensity);

	m_actors.push_back(m_plunger);
}

void Pinball::InitCornerWedges()
{
	Log::Write("\tInitializing Corner Wedges...\n", ENGINE_LOG);

	Fl32 zOffset, xOffset, yOffset;
	Transform pose;
	zOffset = xOffset = yOffset = 0.f;
	pose = Transform::createIdentity();

	Vec3 scale = Vec3(.3f, .3f, .05f);
	Vec3 color = Vec3(.5f, .5f, .5f);
	PxMaterial* mat = PHYSICS->createMaterial(0.f, 0.f, .1f);

	// Top Right Wedge
	zOffset = board->Top().z - board->WallWidth() - .35f;
	xOffset = board->Right().x + board->WallWidth() + .25f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset+(scale.z*2), zOffset), Quat(DEG2RAD(180), Vec3(0, 1, 0)) * Quat(DEG2RAD(25), Vec3(1, 0, 0)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));

	// Top Left Wedge
	zOffset = board->Top().z - board->WallWidth() - .5f;
	xOffset = board->Left().x - board->WallWidth() - .08f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-90), Vec3(0, 1, 0)) * Quat(DEG2RAD(25), Vec3(0, 0, 1)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));

	// Bottom Left Wedge
	zOffset = board->Bottom().z - board->WallWidth() + .3f;
	xOffset = board->Left().x - board->WallWidth() - .35f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));

	// Bottom Right Wedge
	scale = Vec3(.2f, .4f, .05f);
	zOffset = board->Bottom().z + board->WallWidth() + .35f;
	xOffset = board->Right().x + board->WallWidth() + .35f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)) * Quat(DEG2RAD(90), Vec3(0, 1, 0)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));

	// Exit Plunger Lane Wedge
	scale = Vec3(.4f, .4f, .05f);
	zOffset = board->Top().z - board->WallWidth() - 1.7f;
	xOffset = board->Left().x - board->WallWidth() - .45f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));
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

Fl32 Pinball::calcYOffset(Fl32 zOffset)
{
	return tanf(DEG2RAD(25)) * zOffset;
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(gameState)
	{
	case GameState::Menu:
		break;
	case GameState::InGame:
		// Update Camera
		camera.Update();

		// Render Scene
		std::vector<physx::PxRigidActor*> actors = m_scene->GetActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC | physx::PxActorTypeSelectionFlag::eRIGID_STATIC); // get scene actors
	
		int nbActors = actors.size();
		PxShape* shapes[MAX_NUM_ACTOR_SHAPES]; // Pointer to current actor shapes

		if(actors.size())
		{
			for(int i = 0; i < nbActors; i++)
			{
				if(i != GLASS_ATR_IDX) // Don't Render the glass
				{
					PxU32 nbShapes = actors[i]->getNbShapes();
					if(nbShapes <= MAX_NUM_ACTOR_SHAPES)
					{
						actors[i]->getShapes(shapes, nbShapes);

						bool isSleeping = actors[i]->isRigidDynamic() && actors[i]->isRigidDynamic()->isSleeping(); // Check if actor is sleeping

						for(int j = 0; j < nbShapes; j++)
						{
							Transform p = PxShapeExt::getGlobalPose(*shapes[j], *actors[i]);
							PxGeometryHolder h = shapes[j]->getGeometry();

							Mat44 pose(p); // Create Matrix from vector

							glPushMatrix();
							glMultMatrixf((Fl32*)&pose); // Multiply current matrix by pose

							Vec3 aColor = *((Vec3*)actors[i]->userData);
							glColor3f(aColor.x, aColor.y, aColor.z);

							if(h.getType() == PxGeometryType::ePLANE)
								glDisable(GL_LIGHTING);

							Game::RenderGeometry(h);

							if(h.getType() == PxGeometryType::ePLANE)
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
		break;
	}

	hud.Render(camera.FOV);
	glutSwapBuffers();
	Game::Render();
}

void Pinball::Idle()
{
	Game::Idle();

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
		m_ballsRemaining--;
		m_ballInPlay = false;
		m_ball->Get().dynamicActor->setGlobalPose(m_ballInitialPos);
		hud.UpdateItem("Balls Left", m_ballsRemaining);
		if (m_ballsRemaining < 0)
		{
			gameState = GameState::GameOver;
			InitHUD();
		}
	}
}

void Pinball::Reshape(int width, int height)
{
	Game::Reshape(width, height);
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

void Pinball::Exit()
{
	Game::Exit();

	RELEASE(m_ball);
	RELEASE(board);
}