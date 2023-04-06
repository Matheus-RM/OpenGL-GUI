#include "Util/GLHelper.hpp"

static const std::unordered_map<GLenum, const char*> GLMessageEnumToText = {
	// this was getted in https://www.khronos.org/opengl/wiki/Debug_Output

	// message sources
	{GL_DEBUG_SOURCE_API, "API"},
	{GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
	{GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
	{GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
	{GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
	{GL_DEBUG_SOURCE_OTHER, "OTHER"},

	// message types
	{GL_DEBUG_TYPE_ERROR, "ERROR"},	
	{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
	{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
	{GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
	{GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
	{GL_DEBUG_TYPE_MARKER, "MARKER"},
	{GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
	{GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
	{GL_DEBUG_TYPE_OTHER, "OTHER"},

	// message severity
	{GL_DEBUG_SEVERITY_HIGH, "HIGH"},
	{GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
	{GL_DEBUG_SEVERITY_LOW, "LOW"},
	{GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"},
};

static const std::unordered_map<GLenum, size_t> GLTypesToText = {

	{GL_BYTE,			 	sizeof(GLbyte)},
	{GL_UNSIGNED_BYTE,		sizeof(GLubyte)},
	{GL_SHORT,			 	sizeof(GLshort)},
	{GL_UNSIGNED_SHORT,		sizeof(GLushort)},
	{GL_INT,			 	sizeof(GLint)},
	{GL_UNSIGNED_INT,		sizeof(GLuint)},
	{GL_FLOAT,			 	sizeof(GLfloat)},
	{GL_DOUBLE,			 	sizeof(GLdouble)},
	{GL_FIXED,				sizeof(GLfixed)},
	{GL_HALF_FLOAT,			sizeof(GLhalf)},
};

void GLAPIENTRY GL::Help::errorMessageCallback(
									GLenum source, GLenum type, GLuint id, GLenum severity,
									GLsizei length, const GLchar* message, const void* userParam)
{
	const auto& sourceText = GLMessageEnumToText.at(source);
	const auto& typeText = GLMessageEnumToText.at(type);
	const auto& severityText = GLMessageEnumToText.at(severity);

	fprintf(stderr, "GL Callback: from = %s, type = %s, severity = %s, message = \"%s\".\n",
			sourceText, typeText, severityText, message);
}

size_t GL::Help::getSizeOfType(GLenum type)
{
	return GLTypesToText.at(type);
}
