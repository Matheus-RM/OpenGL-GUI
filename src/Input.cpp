#include "Input.hpp"

Input::Input()
{

}

Input::~Input()
{

}

void Input::handleEvents(SDL_Window* window)
{
	while(SDL_PollEvent(&mEvent))
	{
		if(mEvent.type == SDL_QUIT
			|| (mEvent.type == SDL_WINDOWEVENT
				&& mEvent.window.event == SDL_WINDOWEVENT_CLOSE
				&& mEvent.window.windowID == SDL_GetWindowID(window)))
		{
			mRunningState = false;
		}
		else if(mEvent.type == SDL_MOUSEMOTION)
		{
			mMousePosition = {mEvent.motion.x, mEvent.motion.y};
			mMouseMotionOcurried = true;
		}
	}
}

bool Input::isRunning() const
{
	return mRunningState;
}

void Input::lockMouse() const
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Input::unlockMouse() const
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

glm::ivec2 Input::getMousePosition() const
{
	return mMousePosition;
}

bool Input::mouseMotionOcurried()
{
	if(mMouseMotionOcurried)
	{
		mMouseMotionOcurried = false;
		return true;
	}

	return false;
}
