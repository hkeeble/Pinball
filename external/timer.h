#ifndef _TIMER_H_
#define _TIMER_H_

namespace GameFramework
{
	class Timer
	{
	private:
		int currentMilliseconds;
	public:
		Timer();
		Timer(const Timer& param);
		Timer(const int& hours, const int& minutes, const int& seconds, const int& milliseconds);
		~Timer();
		Timer& operator=(const Timer& param);

		void Set(const int& hours, const int& minutes, const int& seconds, const int& milliseconds);

		void Reset();
		void Update(int deltaTime);

		int Milliseconds();
		int Seconds();
		int Minutes();
		int Hours();
	};
}

#endif // _TIMER_H_