#pragma once

#include "Base/Camera.hpp"

class CameraManager
{
	private:
		CameraManager();
		~CameraManager();

	public:
		CameraManager(const CameraManager&) = delete;
		void operator=(const CameraManager&) = delete;

		static CameraManager& getInstance();

	public:
		void setCamera2D(const glm::vec2& position, const glm::vec2& viewSize, float zoom);

		void translate(const glm::vec2& offset);

		void setPosition(const glm::vec2& position);

		void translateX(float offset);
		void translateY(float offset);

		void setViewSize(const glm::vec2& viewSize);
		void setZoom(float zoom);

		glm::ivec2 getPosition2D() const;
		glm::ivec2 getCenteredPosition2D() const;
		
		glm::vec2 getViewSize() const;
		float getZoom() const;

		glm::mat4 getOrthoViewMatrix() const;
		void calculateOrthoMatrix();

	private:
		glm::mat4 calculateViewMatrix2D() const;

	private:
		glm::mat4 mOrthoMatrix;

		CameraData2D mCamera2D;
};