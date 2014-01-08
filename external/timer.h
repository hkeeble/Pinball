#ifndef _TIMER_H_
#define _TIMER_H_

namespace GameFramework
{
	class Timer
	{
	private:
		double currentMilliseconds;
	public:
		Timer();
		Timer(const Timer& param);
		Timer(const int& hours, const int& minutes, const double& seconds, const double& milliseconds);
		~Timer();
		Timer& operator=(const Timer& param);

		void Set(const int& hours, const int& minutes, const double& seconds, const double& milliseconds);

		void Reset();
		void Update(double deltaTime);

		double Milliseconds();
		double Seconds();
		int Minutes();
		int Hours();
	};
}

#endif // _TIMER_H_