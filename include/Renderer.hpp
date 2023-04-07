#pragma once

#include "Base/Shape.hpp"
#include "Base/Shader.hpp"

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_uint4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include <vector>

class Renderer
{
	public:
		Renderer();
		~Renderer();

	public:
		void start(const glm::ivec2& size);

		void draw(const std::vector<ShapeData>& shapes, size_t validSize) const;

		void setClearColor(const glm::vec3& color);

		unsigned int getShapeId(const glm::ivec2& position) const;

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
		void drawShape(const ShapeData& shape) const;
		void blitScreenBuffer() const;

		unsigned int getShapeIdDefault(const glm::ivec2& position) const;
		unsigned int getShapeIdMSAA(const glm::ivec2& position) const;

	private:
		glm::ivec2 mSize;
		glm::vec4 mClearColor;
		const glm::uvec4 mIdClearColor = {0, 0, 0, 0};

		Shader mShader;
		GLuint mFrameBuffer;
		GLuint mScreenRBO;
		GLuint mDepthRBO;
		GLuint mIdTexture;

		bool mIsAntiAliasingSupported;
		int mSamples;
};