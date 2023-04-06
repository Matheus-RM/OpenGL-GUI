#include "Time/Timer.hpp"

#include <SDL2/SDL_timer.h>

Timer::Timer()
{

}

Timer::~Timer()
{

}

void Timer::onFrameStart()
{
	mFrameStartTime = SDL_GetTicks();
}

void Timer::onFrameEnd()
{
	mDeltaTime = (SDL_GetTicks() - mFrameStartTime) / 1000.f;

	mFpsSums += 1 / mDeltaTime;
	mFrameCount++;

	if(mFrameCount >= FPS_SAMPLES)
	{
		mFps = mFpsSums / FPS_SAMPLES;
		mFpsSums = 0.f;
		mFrameCount = 0;
	}
}

float Timer::getDeltaTime() const
{
	return mDeltaTime;
}

float Timer::getFps() const
{
	return mFps;
}
