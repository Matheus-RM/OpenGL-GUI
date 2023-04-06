#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_int2.hpp>
#include <vector>

namespace GL::Factory
{
	template <class T>
	GLuint createBuffer(const GLenum& type, unsigned int size, const T* data, const GLenum& storeMode)
	{
		GLuint buffer;
		glCreateBuffers(1, &buffer);
		glBindBuffer(type, buffer);

		glBufferData(type, size, data, storeMode);

		return buffer;
	}

	GLuint createBuffer(const GLenum& type);
	GLuint reserveBuffer(const GLenum& type, unsigned int size, const GLenum& storeMode);

	void resizeBuffer(const GLuint& buffer, const GLenum& type, unsigned int size,
									const GLenum& storeMode);

	template <class T>
	void resizeBuffer(const GLuint& buffer, const GLenum& type, unsigned int size,
									const T* data, const GLenum& storeMode)
	{
		glBindBuffer(type, buffer);
		glBufferData(type, size, data, storeMode);
	}

	GLuint createVAO();
	void configureVAO(GLuint vao, const std::vector<unsigned int>& elementsPerVertex, unsigned int vertices);
	void configureVAO( GLuint vao, 
						  const std::vector<unsigned int>& count,
						  const std::vector<GLenum>& glType,
						  unsigned int vertices);

	void deleteBuffer(GLuint* buffer);
	void deleteVAO(GLuint* vao);

	void unbindVAO();

	GLuint createRenderbuffer(const glm::ivec2& size, GLenum internalFormat);
	GLuint createRenderbufferMultisample(const glm::ivec2& size, GLenum internalFormat, int samples);

	GLuint createTexture2D(const glm::ivec2& size, GLenum internalFormat, GLenum format, GLenum type);
	GLuint createTexture2DMultisample(const glm::ivec2& size, GLenum internalFormat, int samples);

	void attachRenderBuffer(GLuint renderBuffer, GLenum attachment);
	void attachTexture2D(GLuint texture, GLenum attachment, GLenum textureType, int level = 0);
};