#pragma once

#include <GL/glew.h>

#include <glm/mat4x4.hpp>

#include <string>
#include "Base/Shader.hpp"

#define __UNIFORM_SETTER(TYPE) \
	void setUniform(const struct Shader& shader, const std::string& name, TYPE value);

#define UNIFORM_SETTER_CONSTREF(TYPE) __UNIFORM_SETTER(const TYPE&)

#define UNIFORM_SETTER_TYPE(TYPE) __UNIFORM_SETTER(TYPE)

#define UNIFORM_SETTER_MATRIX(TYPE) UNIFORM_SETTER_CONSTREF(TYPE);
	
#define UNIFORM_SETTER_VEC(TYPE) UNIFORM_SETTER_CONSTREF(TYPE);

namespace GL::Shader
{

	UNIFORM_SETTER_MATRIX(glm::mat4);
	UNIFORM_SETTER_VEC(glm::ivec2);
	UNIFORM_SETTER_VEC(glm::vec2);

	UNIFORM_SETTER_TYPE(float);
	UNIFORM_SETTER_TYPE(int);
	UNIFORM_SETTER_TYPE(unsigned int);

	void setSamplerLocation(const struct Shader& shader, const std::string& name, int index);

	GLint getLocation(GLuint program, const std::string& name);

	void activate(const struct Shader& shader);
};