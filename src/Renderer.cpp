#include "Renderer.hpp"

#include "ShaderLoader.hpp"
#include "CameraManager.hpp"
#include "GLShader.hpp"
#include "Factories/GLFactory.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/gl.h>
#include <stdexcept>

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	glDeleteTextures(1, &mIdTexture);
	glDeleteRenderbuffers(1, &mScreenRBO);
	glDeleteRenderbuffers(1, &mDepthRBO);

	glDeleteFramebuffers(1, &mFrameBuffer);
}

void Renderer::start(const glm::ivec2& size)
{
	mSize = size;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	loadShader();
	createFrameBuffer();
}

void Renderer::loadShader()
{
	auto& loader = ShaderLoader::getInstance();

	loader.readShader("default", { {"./shaders/default_vertex.glsl", GL_VERTEX_SHADER},
								   {"./shaders/default_fragment.glsl", GL_FRAGMENT_SHADER} });

	mShader = loader.getShader("default");
}

void Renderer::createFrameBuffer()
{
	glGenFramebuffers(1, &mFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);

	resolveAntiAliasingSupport();
	configureFrameBuffer();

	setDrawBuffers();

	// unbind fbo
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::resolveAntiAliasingSupport()
{
	int maxSamples, maxIntegerSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &maxIntegerSamples);

	if(maxSamples == 0 || maxIntegerSamples == 0)
	{
		mIsAntiAliasingSupported = false;
		mSamples = 0;
		return;
	}

	mIsAntiAliasingSupported = true;
	glEnable(GL_MULTISAMPLE);

	if(maxSamples >= 4 && maxIntegerSamples >= 4)
	{
		mSamples = 4;
		return;
	}

	mSamples = std::min(maxSamples, maxIntegerSamples);
}

void Renderer::configureFrameBuffer()
{
	if(mIsAntiAliasingSupported)
		configureFrameBufferMSAA();
	else
		configureFrameBufferDefault();
}

void Renderer::configureFrameBufferDefault()
{
	mScreenRBO = GL::Factory::createRenderbuffer(mSize, GL_RGBA);
	GL::Factory::attachRenderBuffer(mScreenRBO, GL_COLOR_ATTACHMENT0);

	mIdTexture = GL::Factory::createTexture2D(mSize, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
	GL::Factory::attachTexture2D(mIdTexture, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D);

	mDepthRBO = GL::Factory::createRenderbuffer(mSize, GL_DEPTH_COMPONENT24);
	GL::Factory::attachRenderBuffer(mDepthRBO, GL_DEPTH_ATTACHMENT);
}

void Renderer::configureFrameBufferMSAA()
{
	mScreenRBO = GL::Factory::createRenderbufferMultisample(mSize, GL_RGBA, mSamples);
	GL::Factory::attachRenderBuffer(mScreenRBO, GL_COLOR_ATTACHMENT0);

	mIdTexture = GL::Factory::createTexture2DMultisample(mSize, GL_R32UI, mSamples);
	GL::Factory::attachTexture2D(mIdTexture, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE);

	mDepthRBO = GL::Factory::createRenderbufferMultisample(mSize, GL_DEPTH_COMPONENT24, mSamples);
	GL::Factory::attachRenderBuffer(mDepthRBO, GL_DEPTH_ATTACHMENT);
}

void Renderer::setDrawBuffers() const
{
	const GLenum drawBuffers [] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, drawBuffers);
}


void Renderer::draw() const
{
	auto& cameraManager = CameraManager::getInstance();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);

	clearBuffers();

	GL::Shader::activate(mShader);
	GL::Shader::setUniform(mShader, "orthoViewMat", cameraManager.getOrthoViewMatrix());

	for(auto& shape : mShapes)
	{
		drawShape(shape);
	}

	blitScreenBuffer();
}

void Renderer::clearBuffers() const
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(mClearColor));
	glClearBufferuiv(GL_COLOR, 1, glm::value_ptr(mIdClearColor));
}

void Renderer::drawShape(const Shape& shape) const
{
	GL::Shader::setUniform(mShader, "shapeId", shape.id);
	GL::Shader::setUniform(mShader, "priority", shape.priority);
	GL::Shader::setUniform(mShader, "radius", shape.borderRadius);
	GL::Shader::setUniform(mShader, "size", shape.size);
	GL::Shader::setUniform(mShader, "modelMat", shape.modelMatrix);

	glBindVertexArray(shape.vao);
	glDrawElements(GL_TRIANGLE_STRIP, shape.indices, GL_UNSIGNED_INT, nullptr);
}

void Renderer::blitScreenBuffer() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	glBlitFramebuffer(0, 0, mSize.x, mSize.y, 0, 0, mSize.x, mSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


void Renderer::setClearColor(const glm::vec3& color)
{
	mClearColor = glm::vec4(color, 1.0f);
}

void Renderer::createSquare(const glm::vec2& position, const glm::vec2& size, unsigned int priority,
							int borderRadius, Background::BackgroundData background)
{
	const unsigned int indices [] = {
		0, 1, 2, 0, 2, 3
	};

	const float vertices [] = {
		0.0f, 0.0f,
		size.x, 0.0f,
		size.x, size.y,
		0.0f, size.y,
	};

	Shape result = {
		.indices = 6,
		.id = mNextId++,
		.priority = -1.0f + ((float)priority * 2.0f / (float)mMaxPriority), // clamp to the range [-1, 1]
		.size = size,
		.borderRadius = borderRadius,
		.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)),
	};

	result.vao = GL::Factory::createVAO();
	result.vbo = GL::Factory::reserveBuffer(GL_ARRAY_BUFFER,
											sizeof(vertices) + background->size() * sizeof(glm::vec4),
											GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), background->size() * sizeof(glm::vec4),
					glm::value_ptr((*background)[0]));

	result.ebo = GL::Factory::createBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
											&indices, GL_STATIC_DRAW);

	GL::Factory::configureVAO(result.vao, {2, 4}, 4);

	mShapes.push_back(result);
}

unsigned int Renderer::getShapeId(const glm::ivec2& position)
{
	if(mIsAntiAliasingSupported)
		return getShapeIdMSAA(position);

	return getShapeIdDefault(position);
}

unsigned int Renderer::getShapeIdDefault(const glm::ivec2& position)
{
	unsigned int result = 0;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);

	glReadBuffer(GL_COLOR_ATTACHMENT1);

	glReadPixels(position.x, mSize.y - position.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &result);

	return result;
}

unsigned int Renderer::getShapeIdMSAA(const glm::ivec2& position)
{
	return 0;
}

void Renderer::setMaxPriority(unsigned int level)
{
	mMaxPriority = level;
}
