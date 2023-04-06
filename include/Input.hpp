#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

#include <glm/ext/vector_int2.hpp>

class Input
{
	public:
		Input();
		~Input();

	public:

		void handleEvents(SDL_Window* window);
		bool isRunning() const;

		void lockMouse() const;
		void unlockMouse() const;

		glm::ivec2 getMousePosition() const;
		bool mouseMotionOcurried();

	private:
		bool mRunningState = true;
		SDL_Event mEvent;

		glm::ivec2 mMousePosition {};
		bool mMouseMotionOcurried = false;
};
