#include "CameraManager.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

CameraManager::CameraManager()
{

}

CameraManager::~CameraManager()
{
	
}

CameraManager& CameraManager::getInstance()
{
	static CameraManager instance;

	return instance;
}

void CameraManager::setCamera2D(const glm::vec2& position, const glm::vec2& viewSize, float zoom)
{
	setPosition(position);
	setViewSize(viewSize);
	setZoom(zoom);
}

void CameraManager::translate(const glm::vec2& offset)
{
	mCamera2D.position += glm::vec3(offset, 0.f);
}


void CameraManager::setPosition(const glm::vec2& position)
{
	mCamera2D.position = glm::vec3(position, 0.f);

}

void CameraManager::translateX(float offset)
{
	mCamera2D.position.x += offset;
}

void CameraManager::translateY(float offset)
{
	mCamera2D.position.y += offset;
}


// 2D functions

void CameraManager::setViewSize(const glm::vec2& viewSize)
{
	mCamera2D.viewSize = viewSize;
}

void CameraManager::setZoom(float zoom)
{
	mCamera2D.zoom = zoom;
}


glm::ivec2 CameraManager::getPosition2D() const
{
	return glm::ivec2(mCamera2D.position);
}

glm::ivec2 CameraManager::getCenteredPosition2D() const
{
	return getPosition2D() + glm::ivec2(mCamera2D.viewSize / 2.0f);
}


glm::vec2 CameraManager::getViewSize() const
{
	return mCamera2D.viewSize;
}

float CameraManager::getZoom() const
{
	return mCamera2D.zoom;
}


glm::mat4 CameraManager::getOrthoViewMatrix() const
{
	return mOrthoMatrix * calculateViewMatrix2D();
}


glm::mat4 CameraManager::calculateViewMatrix2D() const
{
	return glm::translate(glm::mat4(1.0f), -mCamera2D.position);
}

void CameraManager::calculateOrthoMatrix()
{
	const auto&& zoomView = mCamera2D.viewSize * mCamera2D.zoom;
	mOrthoMatrix = glm::ortho(0.0f, zoomView.x, zoomView.y, 0.0f);
}