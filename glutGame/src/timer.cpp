#include "timer.h"

namespace GameFramework
{
	Timer::Timer()
	{
		currentMilliseconds = 0;
	}

	Timer::Timer(const Timer& param)
	{
		currentMilliseconds = param.currentMilliseconds;
	}

	Timer::Timer(const int& hours, const int& minutes, const double& seconds, const double& milliseconds)
	{
		Set(hours, minutes, seconds, milliseconds);
	}

	Timer::~Timer()
	{

	}

	Timer& Timer::operator=(const Timer& param)
	{
		if (&param == this)
			return *this;
		else
		{
			currentMilliseconds = param.currentMilliseconds;
			return *this;
		}
	}

	void Timer::Set(const int& hours, const int& minutes, const double& seconds, const double& milliseconds)
	{
		currentMilliseconds = ((hours * 60) * 60) + (minutes * 60) + (seconds * 1000) + milliseconds;
	}

	void Timer::Reset()
	{
		currentMilliseconds = 0;
	}

	void Timer::Update(double deltaTime)
	{
		currentMilliseconds += deltaTime;
	}

	double Timer::Milliseconds()
	{
		return currentMilliseconds;
	}

	double Timer::Seconds()
	{
		return currentMilliseconds * 1000;
	}

	int Timer::Minutes()
	{
		return Seconds() / 60;
	}

	int Timer::Hours()
	{
		return Minutes() / 60;
	}
}