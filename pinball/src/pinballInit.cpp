/*-------------------------------------------------------------------------\
| File: PINBALLINIT.CPP														|
| Desc: Provides implementations for a pinball game's intialization			|
|		functions.															|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "util.h"

void Pinball::Init()
{
	// Not Paused
	m_paused = false;

	// Set Time Step
	m_pxTimeStep = 1.f / FPS;

	// Set State
	gameState = GameState::Menu;

	// Initialize Camera
	Init2DCamera();

	// Set Clear Color
	SetClearColor(GetClearColor());

	// Set Images
	titleImg = Image("titleTexture.png");
	gameOverImg = Image("gameOverTexture.png");
	aboutImg = Image("about.png");
	instructionImg = Image("instructions.png");
	backgroundImg = Image("backgroundTexture.png");

	// Initialize Scene
	InitGame();
}

void Pinball::InitGame()
{
	// Initialize All Actors
	Log::Write("Initializing Game Actors...\n", ENGINE_LOG);
	InitBoard();
	InitInnerWalls();
	InitFlippers();
	InitBall();
	InitPlunger();
	InitCornerWedges();
	InitCenterBumpers();
	InitSpinners();

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

	// Set Simulation Callback
	m_scene->SetEventCallback(new ScoreCallback());
}

void Pinball::InitHUD()
{
	hud.Clear();

	switch (gameState)
	{
	case GameState::InGame:
		SetClearColor(Vec3(0.f, 0.f, 0.f));
		hud.SetRenderColor(Vec3(1.f, 1.f, 1.f));
		hud.AddItem("Score", Vec2(5, 95), HUDFont::largeFont, true, 0);
		hud.AddItem("Balls Left", Vec2(65, 95), HUDFont::largeFont, true, m_ballsRemaining);
		hud.AddItem("FPS", Vec2(5, 5), HUDFont::largeFont, true, m_fps);
		break;
	case GameState::GameOver:
		SetClearColor(Vec3(0.f, 0.f, 0.f));
		hud.SetRenderColor(Vec3(1.f, 1.f, 1.f));
		hud.AddItem("Score", Vec2(45, 75), HUDFont::smallFont, true, m_currentScore);
		hud.AddItem("Game Duration (Seconds)", Vec2(25, 65), HUDFont::smallFont, true, m_gameDuration.Seconds());
		break;
	case GameState::Paused:
		hud.SetRenderColor(Vec3(1.f, 1.f, 1.f));
		hud.AddItem("PAUSED", Vec2(40, 40), HUDFont::largeFont);
		break;
	}
}

void Pinball::Init3DCamera()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera = { UP_VECTOR, Vec3(0, 0, 0), Vec3(0, 1, -5.5), DEFAULT_FOV };
	camera.Update(); // Used to initialize camera positions (gluLookAt)
	UpdatePerspective(camera.FOV);

	m_is2D = false;
}

void Pinball::Init2DCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_is2D = true;
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
	Transform lftFPos = Transform(Vec3(.28f, yOffset + FlipperDimensions.y * 2, zOffset));
	Transform rgtFPos = Transform(Vec3(-.28f, yOffset + FlipperDimensions.y * 2, zOffset));

	// Transform to center
	const Transform transPos = Transform(Vec3(0.05f, 0, 0));
	lftFPos = lftFPos * Transform(Vec3(0.06f, 0, 0));
	rgtFPos = rgtFPos * Transform(Vec3(0.06f, 0, 0));

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
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(180), Vec3(0, 1, 0)) * Quat(DEG2RAD(25), Vec3(1, 0, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));
	
	// Top Left Wedge
	zOffset = board->Top().z - board->WallWidth() - .5f;
	xOffset = board->Left().x - board->WallWidth() - .08f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-90), Vec3(0, 1, 0)) * Quat(DEG2RAD(25), Vec3(0, 0, 1)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));

	// Bottom Left Wedge
	zOffset = board->Bottom().z - board->WallWidth() + .3f;
	xOffset = board->Left().x - board->WallWidth() - .35f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));

	// Bottom Right Wedge
	scale = Vec3(.2f, .4f, .05f);
	zOffset = board->Bottom().z + board->WallWidth() + .35f;
	xOffset = board->Right().x + board->WallWidth() + .35f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)) * Quat(DEG2RAD(90), Vec3(0, 1, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));

	// Exit Plunger Lane Wedge
	scale = Vec3(.2f, .4f, .05f);
	zOffset = board->Top().z - board->WallWidth() - 1.7f;
	xOffset = board->Left().x - board->WallWidth() - .25f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));

	// Flipper Right Wedge
	scale = Vec3(.5f, .4f, .05f);
	zOffset = board->Bottom().z + board->WallWidth() + .6f;
	xOffset = board->Left().x - board->WallWidth() - .55f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));

	// Flipper Right Wedge
	scale = Vec3(.2f, .8f, .05f);
	zOffset = board->Bottom().z + board->WallWidth() + .76f;
	xOffset = board->Right().x - board->WallWidth() + .49f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0))  * Quat(DEG2RAD(90), Vec3(0, 1, 0)));
	m_actors.push_back(ConvexMeshActor::CreateWedge(pose, 1.f, color, mat, scale, Physics::ActorType::StaticActor));
}

void Pinball::InitCenterBumpers()
{
	// Variables
	ConvexMeshActor* currentActor; // The current actor
	Fl32 zCenter, xCenter, zAbs, xAbs; // Center of bumpers, absolute position of current actor
	Transform rotation = Transform(Quat(DEG2RAD(-115), Vec3(1, 0, 0)));

	// Actor Parameters
	Vec3 color = Vec3(1, 1, .5f);
	PxMaterial* material = PHYSICS->createMaterial(0, 0, 2.f);
	Fl32 density = 1.f;
	Vec3 scale = Vec3(0.2f, 0.1f, 0.2f);
	Vec3 scale2 = scale*0.8f;

	zCenter = board->Center().z + 1.f;
	xCenter = board->Center().x;

	zAbs = zCenter;
	xAbs = xCenter - .7f;
	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale, ActorType::DynamicActor);
	currentActor->IsTrigger(true);
	m_actors.push_back(currentActor);

	// Actor used for bounce...
	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale2, ActorType::StaticActor);
	m_actors.push_back(currentActor);

	zAbs = zCenter;
	xAbs = xCenter + .7f;
	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale, ActorType::DynamicActor);
	currentActor->IsTrigger(true);
	m_actors.push_back(currentActor);

	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale2, ActorType::StaticActor);
	m_actors.push_back(currentActor);

	zAbs = zCenter - .4f;
	xAbs = xCenter;
	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale, ActorType::DynamicActor);
	currentActor->IsTrigger(true);
	m_actors.push_back(currentActor);

	currentActor = ConvexMeshActor::CreatePyramid(CreatePosition(xAbs, zAbs) * Transform(Vec3(0, 0.1f, 0)) * rotation, density, color, material, scale2, ActorType::StaticActor);
	m_actors.push_back(currentActor);
}

void Pinball::InitSpinners()
{
	// Actor Parameters
	Spinner *lft, *rgt;
	Vec3 color = Vec3(1.f, .0f, .0f);
	PxMaterial* material = PHYSICS->createMaterial(0, 0, 0);
	Fl32 density = .5f;
	Transform pose;

	Fl32 xOffset, zOffset;

	// Left Spinner
	xOffset = board->Center().x - 0.7f;
	zOffset = board->Center().z;
	pose = Transform(Vec3(xOffset, calcYOffset(zOffset) + (board->WallHeight()), zOffset));
	lft = new Spinner(pose, material, color, density);
	m_actors.push_back(lft);

	// Right Spinner
	xOffset = board->Center().x + 0.8f;
	zOffset = board->Center().z;
	pose = Transform(Vec3(xOffset, calcYOffset(zOffset) + (board->WallHeight()), zOffset));
	rgt = new Spinner(pose, material, color, density);
	m_actors.push_back(rgt);

	// Create Spinners Object
	m_spinners = new Spinners(lft, rgt);
}