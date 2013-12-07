#include "hud.h"


HUDItem::HUDItem()
{
	m_text = nullptr;
	m_data = 0;
	m_pos = Vec2(0, 0);
	m_font = HUDFont::smallFont;
}

HUDItem::HUDItem(std::string text, int data, Vec2 pos, HUDFont font)
{
	m_text = text;
	m_data = data;
	m_pos = pos;
	m_font = font;
}

HUDItem::~HUDItem()
{

}

HUD::HUD()
{

}

HUD::~HUD()
{

}

void HUD::Render()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glColor3f(textColor.x, textColor.y, textColor.z);

		for (std::vector<HUDItem>::iterator iter = m_items.begin(); iter < m_items.end(); iter++)
		{
			glRasterPos2f(iter->m_pos.x, iter->m_pos.y);
			for (int i = 0; i < iter->m_text.length; i++)
				glutBitmapCharacter((void*)iter->m_font, iter->m_text[i]);
		}

		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective();
	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void HUD::AddItem(std::string text, int initialData)
{
	m_items.push_back(HUDItem(text, initialData));
}

bool HUD::UpdateItem(std::string text, int newData)
{
	for (std::vector<HUDItem>::iterator iter = m_items.begin(); iter < m_items.end(); iter++)
	{
		if (iter->m_text == text)
		{
			iter->m_data = newData;
			return true;
		}
	}
	return false;
}