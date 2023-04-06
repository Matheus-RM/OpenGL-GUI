#pragma once

#include <GL/glew.h>

#include <SDL2/SDL_video.h>
#include <glm/vec2.hpp>

#include <string>

#include "Base/WindowData.hpp"

class Window
{
	public:
		Window();
		~Window();

	public:
		void start();
		void quit();
		void swap();
		void setConfiguration(const WindowData& configuration);

		void setTitle(const std::string& title);

		SDL_Window* getWindowPointer() const;
		SDL_GLContext getContext() const;

		glm::ivec2 getSize() const;

		void updateViewport() const;

	private:
		void initSDL() const;
		void initIMG() const;
		void initTTF() const;
		void createWindowAndContext();
		void initGlew() const;

		void configureGL() const;

	private:
		SDL_Window* mWindow;
		SDL_GLContext mContext;
		WindowData mConfiguration;
};