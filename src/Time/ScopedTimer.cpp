#include "Time/ScopedTimer.hpp"

#include <iostream>

ScopedTimer::ScopedTimer(const std::string& name) : mName(name)
{
	start();
}

ScopedTimer::~ScopedTimer()
{
	stop();
}

void ScopedTimer::start()
{
	mStart = Clock::getTime();
}

void ScopedTimer::stop()
{
	std::cout << mName << " time: " << Clock::getElapsedTimeMS(mStart) << " ms\n";
}