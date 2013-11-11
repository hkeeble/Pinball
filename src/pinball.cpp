/*-------------------------------------------------------------------------\
| File: PINBALL.CPP															|
| Desc: Provides implementations for a pinball game.						|
| Author: Henri Keeble														|
\-------------------------------------------------------------------------*/
#include "pinball.h"

Pinball::Pinball(std::string title, int windowWidth, int windowHeight, int windowPosX, int windowPosY)
	: Game(title, windowWidth, windowHeight, windowPosX, windowPosY)
{

}

Pinball::~Pinball()
{

}

void Pinball::Init()
{
	m_ball = new Sphere(IDENTITY_TRANS, 0.5f, 1.f, Vec3(1.f, .0f, .0f));
	m_scene->Add(m_ball);

	m_board = new Box(IDENTITY_TRANS, Vec3(0.5f, 0.5f, 0.5f), 1.f, Vec3(0.f, 1.f, 1.f));
	m_scene->Add(m_board);
}

void Pinball::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Scene
	std::vector<physx::PxRigidActor*> actors = m_scene->GetActors(physx::PxActorTypeSelectionFlag::eRIGID_DYNAMIC | physx::PxActorTypeSelectionFlag::eRIGID_STATIC); // get scene actors
	
	int nbActors = actors.size();
	PxShape* shapes[MAX_NUM_ACTOR_SHAPES]; // Pointer to current actor shapes

	if(actors.size())
	{
		for(int i = 0; i < nbActors; i++)
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

					Vec3 defCol = DEFAULT_COLOR;
					glColor3f(defCol.x, defCol.y, defCol.z);

					glPopMatrix();
				}
			}
			else
				Log::Write("Exc: Too many shapes in actor!\n", ENGINE_LOG);
		}
	}

	glutSwapBuffers();
}

void Pinball::Idle()
{
	Game::Idle();
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

}

void Pinball::KeyboardUp(unsigned char key, int x, int y)
{

}

void Pinball::SpecKeyboardDown(int key, int x, int y)
{

}

void Pinball::SpecKeyboardUp(int key, int x, int y)
{

}

void Pinball::Exit()
{
	Game::Exit();

	if(m_ball)
		delete m_ball;
	if(m_board)
		delete m_board;
}