#include "Time/Clock.hpp"

Clock::Clock() 
{

}

Clock::~Clock() 
{
	
}

double Clock::getElapsedTime(time_point start) noexcept
{
	auto&& end = getTime();

	return std::chrono::duration<double, std::ratio<1>>(end - start).count();
}

double Clock::getElapsedTime(time_point start, time_point end) noexcept
{
	return std::chrono::duration<double, std::ratio<1>>(end - start).count();
}

double Clock::getElapsedTimeMS(time_point start) noexcept
{
	auto&& end = getTime();

	return std::chrono::duration<double, std::milli>(end - start).count();
}

double Clock::getElapsedTimeMS(time_point start, time_point end) noexcept
{
	return std::chrono::duration<double, std::milli>(end - start).count();
}


Clock::time_point Clock::getTime() noexcept
{
	return clock::now();
}