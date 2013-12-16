#include "timer.h"

Timer::Timer()
{
	currentMilliseconds = 0;
}

Timer::Timer(const Timer& param)
{
	currentMilliseconds = param.currentMilliseconds;
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

void Timer::Reset()
{
	currentMilliseconds = 0;
}

void Timer::Update(int deltaTime)
{
	currentMilliseconds += deltaTime;
}

int Timer::Milliseconds()
{
	return currentMilliseconds;
}

int Timer::Seconds()
{
	return currentMilliseconds / 1000;
}

int Timer::Minutes()
{
	return Seconds() / 60;
}

int Timer::Hours()
{
	return Minutes() / 60;
}