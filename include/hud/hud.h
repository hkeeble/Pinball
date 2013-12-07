#ifndef _HUD_H_
#define _HUD_H_

#include "uncopyable.h"
#include "globals.h"
#include "game.h"
#include <GL\glut.h>
#include <string>
#include <vector>

enum class HUDFont
{
	largeFont = (int)GLUT_BITMAP_TIMES_ROMAN_24,
	smallFont = (int)GLUT_BITMAP_HELVETICA_18
};

class HUDItem
{
private:
	HUDItem();
	HUDItem(std::string text, int data, Vec2 pos, HUDFont font);
	std::string m_text;
	Vec2 m_pos;
	HUDFont m_font;
	int m_data;
public:
	friend class HUD;

	virtual ~HUDItem();
};

class HUD : private Uncopyable
{
private:
	const Vec3 textColor = Vec3(0.5f, 0.5f, 0.f);

	std::vector<HUDItem> m_items;
public:
	HUD();
	virtual ~HUD();

	void Render();
	void AddItem(std::string text, int initialData);
	bool UpdateItem(std::string text, int newData);
};

#endif // _HUD_H_