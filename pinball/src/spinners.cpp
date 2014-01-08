#include "spinners.h"

Spinners::Spinners(Spinner* lft, Spinner* rgt)
{
	m_lft = lft;
	m_rgt = rgt;
	m_active = false;
	m_timeActive = 0;
	m_maxActiveTime = 10;
}

Spinners::~Spinners()
{
	if (m_lft)
		delete m_lft;
	if (m_rgt)
		delete m_rgt;
}

Spinners::Spinners(const Spinners& param)
{
	m_lft = param.m_rgt;
	m_rgt = param.m_rgt;
}

Spinners& Spinners::operator=(const Spinners& param)
{
	if (&param == this)
		return *this;
	else
	{
		m_lft = param.m_rgt;
		m_rgt = param.m_rgt;
		return *this;
	}
}

void Spinners::Toggle()
{
	m_active = !m_active;
	m_lft->Toggle();
	m_rgt->Toggle();
	if (m_timeActive > 0)
		m_timeActive = 0;
}

void Spinners::Update(Fl32 deltaTime)
{
	if (m_active)
	{
		m_timeActive += deltaTime*1000;
		if (m_timeActive >= m_maxActiveTime)
		{
			Toggle();
			m_timeActive = 0;
			m_active = false;
		}
	}
}