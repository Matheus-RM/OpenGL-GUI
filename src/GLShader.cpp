#include "GLShader.hpp"

#include <glm/gtc/type_ptr.hpp>

#define __IMPL_UNIFORM_SETTER(TYPE, GL_FUNCTION, VALUE_GETTER, ...)		\
	void GL::Shader::setUniform(const struct Shader& shader,			\
								const std::string& name,				\
								TYPE value) 					\
	{																	\
		auto&& location = getLocation(shader.program, name);			\
		GL_FUNCTION(location, ##__VA_ARGS__, VALUE_GETTER(value));		\
	}

#define __IMPL_UNIFORM_SETTER_CONSTREF(TYPE, GL_FUNCTION, VALUE_GETTER, ...)		\
	__IMPL_UNIFORM_SETTER(const TYPE &, GL_FUNCTION, VALUE_GETTER, ##__VA_ARGS__)

#define EMPTY_VALUE_GETTER(VALUE) VALUE

#define IMPL_UNIFORM_SETTER_MAT(TYPE, GL_FUNCTION) 	\
	__IMPL_UNIFORM_SETTER_CONSTREF(TYPE, GL_FUNCTION, glm::value_ptr, 1, GL_FALSE)

#define IMPL_UNIFORM_SETTER_VEC(TYPE, GL_FUNCTION) 	\
	__IMPL_UNIFORM_SETTER_CONSTREF(TYPE, GL_FUNCTION, glm::value_ptr, 1)

#define IMPL_UNIFORM_SETTER_TYPE(TYPE, GL_FUNCTION) \
	__IMPL_UNIFORM_SETTER(TYPE, GL_FUNCTION, EMPTY_VALUE_GETTER)


void GL::Shader::setSamplerLocation(const struct Shader& shader, const std::string &name, int index)
{
	auto&& location = getLocation(shader.program, name);
	glUniform1i(location, index);
}

GLint GL::Shader::getLocation(GLuint program, const std::string& name)
{
	return glGetUniformLocation(program, name.c_str());
}

void GL::Shader::activate(const struct Shader& shader)
{
	glUseProgram(shader.program);
}


IMPL_UNIFORM_SETTER_MAT(glm::mat4, glUniformMatrix4fv);
IMPL_UNIFORM_SETTER_VEC(glm::ivec2, glUniform2iv);
IMPL_UNIFORM_SETTER_VEC(glm::vec2, glUniform2fv);

IMPL_UNIFORM_SETTER_TYPE(float, glUniform1f);
IMPL_UNIFORM_SETTER_TYPE(int, glUniform1i);
IMPL_UNIFORM_SETTER_TYPE(unsigned int, glUniform1ui);