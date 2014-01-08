#include "boardObjects.h"

class Spinners
{
private:
	int m_maxActiveTime;

	Spinner *m_lft, *m_rgt;

	bool m_active;
	Fl32 m_timeActive;
public:
	Spinners(Spinner* lft, Spinner* rgt);
	~Spinners();
	Spinners(const Spinners& param);
	Spinners& operator=(const Spinners& param);

	void Toggle();
	void Update(Fl32 deltaTime);

	bool Active() const;
};