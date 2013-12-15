#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
private:
	int currentMilliseconds;
public:
	Timer();
	Timer(const Timer& param);
	~Timer();
	Timer& operator=(const Timer& param);

	void Reset();
	void Update(int deltaTime);

	int Milliseconds();
	int Seconds();
	int Minutes();
	int Hours();
};

#endif // _TIMER_H_