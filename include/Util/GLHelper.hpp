#pragma once

#include <GL/glew.h>
#include <unordered_map>

namespace GL::Help
{
	void GLAPIENTRY errorMessageCallback(
									GLenum source, GLenum type, GLuint id, GLenum severity,
									GLsizei length, const GLchar* message, const void* userParam);

	size_t getSizeOfType(GLenum type);
}