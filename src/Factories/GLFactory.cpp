#include "Factories/GLFactory.hpp"

#include "Util/GLHelper.hpp"

GLuint GL::Factory::createBuffer(const GLenum& type)
{
	GLuint buffer;
	glCreateBuffers(1, &buffer);
	glBindBuffer(type, buffer);

	return buffer;
}

GLuint GL::Factory::reserveBuffer(const GLenum& type, unsigned int size, const GLenum& storeMode)
{
	GLuint buffer;
	glCreateBuffers(1, &buffer);
	glBindBuffer(type, buffer);

	glBufferData(type, size, nullptr, storeMode);

	return buffer;
}

void GL::Factory::resizeBuffer(const GLuint& buffer, const GLenum& type, unsigned int size,
										const GLenum& storeMode)
{
	glBindBuffer(type, buffer);
	glBufferData(type, size, nullptr, storeMode);
}

GLuint GL::Factory::createVAO()
{
	GLuint result;
	glCreateVertexArrays(1, &result);
	glBindVertexArray(result);

	return result;
}

void GL::Factory::configureVAO(GLuint vao, const std::vector<unsigned int>& elementsPerVertex, unsigned int vertices)
{
	GLuint location = 0;
	uintptr_t currentPointer = 0;

	for(const auto& elements : elementsPerVertex)
	{
		glVertexAttribPointer(location, elements, GL_FLOAT, GL_FALSE, elements*sizeof(float), (void*)currentPointer );
		glEnableVertexArrayAttrib(vao, location);
		
		currentPointer += elements * vertices * sizeof(float) ;
		location++;
	}
}

void GL::Factory::configureVAO( GLuint vao, 
								const std::vector<unsigned int>& count,
								const std::vector<GLenum>& glType,
								unsigned int vertices)
{
	GLuint location = 0;
	uintptr_t currentPointer = 0;

	for(size_t i = 0; i < count.size(); i++)
	{
		const auto&& size = GL::Help::getSizeOfType(glType[i]);
		glVertexAttribPointer(location, count[i], glType[i], GL_FALSE, count[i] * size, (void*)currentPointer );
		glEnableVertexArrayAttrib(vao, location);
		
		currentPointer += count[i] * vertices * sizeof(float) ;
		location++;
	}
}

void GL::Factory::deleteBuffer(GLuint* buffer)
{
	glDeleteBuffers(1, buffer);
}

void GL::Factory::deleteVAO(GLuint* vao)
{
	glDeleteVertexArrays(1, vao);
}


void GL::Factory::unbindVAO()
{
	glBindVertexArray(0);
}


GLuint GL::Factory::createRenderbuffer(const glm::ivec2& size, GLenum internalFormat)
{
	GLuint result;
	glCreateRenderbuffers(1, &result);
	glBindRenderbuffer(GL_RENDERBUFFER, result);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.x, size.y);

	return result;
}

GLuint GL::Factory::createRenderbufferMultisample(const glm::ivec2& size, GLenum internalFormat, int samples)
{
	GLuint result;
	glCreateRenderbuffers(1, &result);
	glBindRenderbuffer(GL_RENDERBUFFER, result);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, size.x, size.y);

	return result;
}


GLuint GL::Factory::createTexture2D(const glm::ivec2& size, GLenum internalFormat, GLenum format, GLenum type)
{
	GLuint result;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, size.x, size.y, 0, format, type, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return result;
}

GLuint GL::Factory::createTexture2DMultisample(const glm::ivec2& size, GLenum internalFormat, int samples)
{
	GLuint result;

	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, result);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, size.x, size.y, GL_TRUE);

	//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return result;
}

void GL::Factory::attachRenderBuffer(GLuint renderBuffer, GLenum attachment)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);
}

void GL::Factory::attachTexture2D(GLuint texture, GLenum attachment, GLenum textureType, int level)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, textureType, texture, level);
}
