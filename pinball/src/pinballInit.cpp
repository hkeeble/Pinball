/*-------------------------------------------------------------------------\
| File: PINBALLINIT.CPP														|
| Desc: Provides implementations for a pinball game's intialization			|
|		functions.															|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"
#include "util.h"

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
	const Transform lftFPos = Transform(Vec3(.26f, yOffset + FlipperDimensions.y * 2, zOffset));
	const Transform rgtFPos = Transform(Vec3(-.12f, yOffset + FlipperDimensions.y * 2, zOffset));

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
	scale = Vec3(.2f, .4f, .05f);
	zOffset = board->Top().z - board->WallWidth() - 1.7f;
	xOffset = board->Left().x - board->WallWidth() - .25f;
	yOffset = calcYOffset(zOffset);
	pose = Transform(Vec3(xOffset, yOffset + (scale.z * 2), zOffset), Quat(DEG2RAD(-25), Vec3(1, 0, 0)));
	m_actors.push_back(new CornerWedge(pose, mat, scale, color));
}