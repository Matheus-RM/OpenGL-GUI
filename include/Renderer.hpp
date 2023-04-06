#pragma once

#include "Base/Shape.hpp"
#include "Base/Shader.hpp"

#include "Background.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_uint4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <functional>
#include <vector>

class Renderer
{
	public:
		Renderer();
		~Renderer();

	public:
		void start(const glm::ivec2& size);

		void draw() const;

		void setClearColor(const glm::vec3& color);

		void createSquare(const glm::vec2& position, const glm::vec2& size, unsigned int priority,
							int borderRadius, Background::BackgroundData background);

		unsigned int getShapeId(const glm::ivec2& position);

		void setMaxPriority(unsigned int level);

	private:
		void loadShader();
		void createFrameBuffer();

		void resolveAntiAliasingSupport();
		void configureFrameBuffer();
		void configureFrameBufferDefault();
		void configureFrameBufferMSAA();
		void setDrawBuffers() const;

		void clearBuffers() const;
		void drawShape(const Shape& shape) const;
		void blitScreenBuffer() const;

		unsigned int getShapeIdDefault(const glm::ivec2& position);
		unsigned int getShapeIdMSAA(const glm::ivec2& position);

	private:
		glm::ivec2 mSize;
		glm::vec4 mClearColor;
		const glm::uvec4 mIdClearColor = {0, 0, 0, 0};

		std::vector<Shape> mShapes;

		Shader mShader;
		GLuint mFrameBuffer;
		GLuint mScreenRBO;
		GLuint mDepthRBO;
		GLuint mIdTexture;

		unsigned int mNextId = 1;

		bool mIsAntiAliasingSupported;
		int mSamples;

		unsigned int mMaxPriority = 1000;
};