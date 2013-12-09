#include "hud.h"

namespace GameFramework
{
	HUDItem::HUDItem()
	{
		m_text = nullptr;
		m_data = 0;
		m_pos = Vec2(0, 0);
		m_font = HUDFont::smallFont;
	}

	HUDItem::HUDItem(std::string text, Vec2 pos, HUDFont font, bool dataItem, int data )
	{
		m_text = text;
		m_data = data;
		m_pos = pos;
		m_font = font;
		m_dataItem = dataItem;
	}

	HUDItem::~HUDItem()
	{

	}

	HUD::HUD()
	{
		m_items = std::vector<HUDItem>();
	}

	HUD::~HUD()
	{
		m_items.clear();
	}

	void HUD::Render(Fl32 FOV)
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
				// Calculate Screen Position
				Vec2 actualPos = Vec2((glutGet(GLUT_WINDOW_WIDTH) / 100) * iter->m_pos.x, (glutGet(GLUT_WINDOW_HEIGHT) / 100) * iter->m_pos.y);
				glRasterPos2f(actualPos.x, actualPos.y);
				for (int i = 0; i < iter->m_text.length(); i++)
					glutBitmapCharacter((void*)iter->m_font, iter->m_text[i]);
				if (iter->m_dataItem)
				{
					glutBitmapCharacter((void*)iter->m_font, ' ');
					glutBitmapCharacter((void*)iter->m_font, ':');
					glutBitmapCharacter((void*)iter->m_font, ' ');
					std::string s = std::to_string(iter->m_data);
					for (int j = 0; j < s.length(); j++)
						glutBitmapCharacter((void*)iter->m_font, s[j]);
				}
			}
			Vec3 clearCol = Game::GetClearColor();
			glColor3f(clearCol.x, clearCol.y, clearCol.z);
			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			Game::UpdatePerspective(FOV); // Reset Projection Matrix
		glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	void HUD::AddItem(std::string text, Vec2 pos, HUDFont font, bool dataItem, int initialData)
	{
		if (pos.x > 100 || pos.y > 100 || pos.x < 0 || pos.y < 0)
		{
			std::string s = "HUD item with text \"" + text + "\" has an invalid position value. Must be 0-100.\n";
			Log::Write(s.c_str(), ENGINE_LOG);
			pos = Vec2(0);
		}

		m_items.push_back(HUDItem(text, pos, font, dataItem, initialData));
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

	void HUD::SetRenderColor(Vec3 color)
	{
		textColor = color;
	}

	void HUD::Clear()
	{
		m_items.clear();
	}
}