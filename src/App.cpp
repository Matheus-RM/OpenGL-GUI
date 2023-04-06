#include "App.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_messagebox.h>

#include <iostream>
#include <stdexcept>

#include "Time/ScopedTimer.hpp"
#include "CameraManager.hpp"

App::App()
{

}

App::~App()
{

}

int App::run()
{
	try
	{
		init();
		update();
		quit();
	}
	catch(std::runtime_error& e)
	{
		std::cout << "Runtime error: " << e.what() << "\n";
		displayMessage("Runtime error", e.what());

		return -1;
	}
	catch(std::exception& e)
	{
		std::cout << "Undefined error: " << e.what() << "\n";
		displayMessage("Undefined error", e.what());

		return -1;
	}

	return 0;
}

void App::init()
{
	ScopedTimer initTimer("Initiation");

	//mProgramLoader.readProgramConfiguration(mProgram);

	mWindow.setConfiguration({"Test Window", 800, 600, 0});

	mWindow.start();
	mWindow.updateViewport();
	
	mRenderer.start(mWindow.getSize());
	mRenderer.setClearColor(glm::vec3(0.95f, 0.95f, 0.95f));

	mRenderer.createSquare({0, 0}, {100, 100}, 1, 0, Background::solid(0.10f, 0.15f, 0.80f));
	mRenderer.createSquare({25, 25}, {100, 100}, 1, 25, Background::solid(0.80f, 0.15f, 0.10f));
	mRenderer.createSquare({50, 50}, {100, 100}, 3, 50, Background::solid(0.10f, 0.80f, 0.15f));

	auto& cameraManager = CameraManager::getInstance();
	cameraManager.setCamera2D({0, 0}, mWindow.getSize(), 1.0f);
	cameraManager.calculateOrthoMatrix();
}

void App::update()
{
	while(mInput.isRunning())
	{
		runFrame();
	}
}

void App::runFrame()
{
	mTimer.onFrameStart();

	mInput.handleEvents(mWindow.getWindowPointer());

	// Frame logic
	//mScriptController.callFrameScripts(mProgram);

	// OpenGL rendering
	//mRenderer->draw(ew, mCameraManager.getProjectionViewMatrix());
	//mWidgets->draw(ew);

	mRenderer.draw();

	auto mousePos = mInput.getMousePosition();
	auto id = mRenderer.getShapeId(mousePos);

	mWindow.setTitle(std::string("Meu App - mouse: (") + std::to_string(mousePos.x) + ", " + 
						std::to_string(mousePos.y) + ") - shape ID: " + std::to_string(id));

	mWindow.swap();

	//mSceneLoader.checkForSceneReading(mProgram);

	mTimer.onFrameEnd();
}

void App::quit()
{
	mWindow.quit();
}

void App::displayMessage(const char* errorType, const char* message)
{
	const SDL_MessageBoxButtonData button = {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Continue"};
	const SDL_MessageBoxColorScheme colorScheme = {
		{
			{250, 250, 250}, // bg
			{0, 0, 0}, // text
			{0, 0, 0}, // button border
			{240, 240, 240}, // button bg
			{255, 255, 255}, // button selected
		}
	};

	const SDL_MessageBoxData data = {
		SDL_MESSAGEBOX_ERROR,
		nullptr,
		errorType,
		message,
		1,
		&button,
		&colorScheme
	};

	int buttonId;
	SDL_ShowMessageBox(&data, &buttonId);
}