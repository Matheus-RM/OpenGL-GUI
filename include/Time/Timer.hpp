#pragma once

#include <SDL2/SDL_stdinc.h>

class Timer
{
	public:
		Timer();
		~Timer();

	public:
		void onFrameStart();
		void onFrameEnd();

		float getDeltaTime() const;
		float getFps() const;

	private:
		Uint32 mFrameStartTime = 0;
		float mFps = 0.0f;
		float mDeltaTime = 0.0f;
		float mFpsSums = 0.0f;
		unsigned int mFrameCount = 0;

		const unsigned int FPS_SAMPLES = 15;
};