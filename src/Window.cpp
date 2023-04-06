#include "Window.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdexcept>
#include <stdio.h>

#include "SDL2/SDL_video.h"
#include "Util/GLHelper.hpp"

Window::Window()
{

}


Window::~Window()
{

}


void Window::start()
{
	initSDL();
	initIMG();
	initTTF();
	createWindowAndContext();
	initGlew();

	configureGL();
}

void Window::quit()
{
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

void Window::swap()
{
	SDL_GL_SwapWindow(mWindow);
}

void Window::setConfiguration(const WindowData& configuration)
{
	mConfiguration = configuration;
}

void Window::setTitle(const std::string& title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

SDL_Window* Window::getWindowPointer() const
{
	return mWindow;
}

SDL_GLContext Window::getContext() const
{
	return mContext;
}

glm::ivec2 Window::getSize() const
{
	return glm::ivec2(mConfiguration.width, mConfiguration.height);
}

void Window::updateViewport() const
{
	glViewport(0, 0, mConfiguration.width, mConfiguration.height);
}

void Window::initSDL() const
{
	const auto&& status = SDL_Init(SDL_INIT_EVERYTHING);

	if(status != 0)
		throw std::runtime_error(
			std::string("Window: could not initialize SDL, SDL message: ") + SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

void Window::initIMG() const
{
	const auto initFlags = IMG_INIT_PNG;
	const auto&& successInitedFlags = IMG_Init(initFlags);

	if((successInitedFlags & initFlags) != initFlags)
		throw std::runtime_error(
			std::string("Window: could not initialize IMG, IMG message: ") + IMG_GetError());
}

void Window::initTTF() const
{
	const auto&& status = TTF_Init();

	if(status == -1)
		throw std::runtime_error(
			std::string("Window: could not initialize TTF, TTF message: ") + TTF_GetError());
}

void Window::createWindowAndContext()
{
	const auto position = SDL_WINDOWPOS_CENTERED;

	mWindow = SDL_CreateWindow(mConfiguration.name.c_str(), position, position,
				mConfiguration.width, mConfiguration.height, mConfiguration.flags | SDL_WINDOW_OPENGL);

	if(mWindow == nullptr)
		throw std::runtime_error(
			std::string("Window: could not create a window, SDL message: ") + SDL_GetError() );

	mContext = SDL_GL_CreateContext(mWindow);
	if(mContext == NULL)
		throw std::runtime_error(
			std::string("Window: could not create a context, SDL message: ") + SDL_GetError() );
}

void Window::initGlew() const
{
	glewInit();
	glewExperimental = true;
}

void Window::configureGL() const
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(&GL::Help::errorMessageCallback, 0);
}