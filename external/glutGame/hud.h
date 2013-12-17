#ifndef _HUD_H_
#define _HUD_H_

#include "uncopyable.h"
#include "globals.h"
#include "glutGame.h"
#include <GL\glut.h>
#include <string>
#include <vector>

namespace GameFramework
{
	enum class HUDFont
	{
		largeFont = (int)GLUT_BITMAP_TIMES_ROMAN_24,
		smallFont = (int)GLUT_BITMAP_HELVETICA_18
	};

	class HUDItem
	{
	private:
		HUDItem();
		HUDItem(std::string text, Vec2 pos, HUDFont font, bool dataItem = false, int data = 0);
		std::string m_text;
		Vec2 m_pos;
		HUDFont m_font;
		int m_data;
		bool m_dataItem;
	public:
		friend class HUD;

		virtual ~HUDItem();
	};

	class HUD : private Uncopyable
	{
	private:
		Vec3 textColor;

		std::vector<HUDItem> m_items;
	public:
		HUD();
		virtual ~HUD();

		void SetRenderColor(Vec3 color);
		void Render(Fl32 FOV);
		void AddItem(std::string text, Vec2 pos, HUDFont font, bool dataItem = false, int initialData = 0);
		bool UpdateItem(std::string text, int newData);
		void Clear();
	};
}
#endif // _HUD_H_
