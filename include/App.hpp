#pragma once

#include "Interface.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Time/Timer.hpp"

class App
{
	public:
		App();
		~App();

	public:
		int run();

	private:
		void init();
		void update();
		void runFrame();
		void quit();

		void displayMessage(const char* errorType, const char* message);

	private:
		Window mWindow;
		Input mInput;
		Timer mTimer;
		Interface mInterface;
};
