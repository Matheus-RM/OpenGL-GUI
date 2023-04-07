#include "Renderer.hpp"

#include "ShaderLoader.hpp"
#include "CameraManager.hpp"
#include "GLShader.hpp"
#include "Factories/GLFactory.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


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


void Renderer::draw(const std::vector<ShapeData>& shapes, size_t validSize) const
{
	auto& cameraManager = CameraManager::getInstance();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);

	clearBuffers();

	GL::Shader::activate(mShader);
	GL::Shader::setUniform(mShader, "orthoViewMat", cameraManager.getOrthoViewMatrix());

	for(size_t i = 0; i < validSize; i++)
	{
		drawShape(shapes.at(i));
	}

	blitScreenBuffer();
}

void Renderer::clearBuffers() const
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(mClearColor));
	glClearBufferuiv(GL_COLOR, 1, glm::value_ptr(mIdClearColor));
}

void Renderer::drawShape(const ShapeData& shape) const
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


unsigned int Renderer::getShapeId(const glm::ivec2& position) const
{
	if(mIsAntiAliasingSupported)
		return getShapeIdMSAA(position);

	return getShapeIdDefault(position);
}

unsigned int Renderer::getShapeIdDefault(const glm::ivec2& position) const
{
	unsigned int result = 0;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);

	glReadBuffer(GL_COLOR_ATTACHMENT1);

	glReadPixels(position.x, mSize.y - position.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &result);

	return result;
}

unsigned int Renderer::getShapeIdMSAA(const glm::ivec2& position) const
{
	return 0;
}
