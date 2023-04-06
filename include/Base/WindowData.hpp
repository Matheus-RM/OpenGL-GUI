#pragma once

#include <string>

#include <SDL2/SDL_stdinc.h>

struct WindowData
{
	std::string name;
	int width;
	int height;
	Uint32 flags;
};