#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct CameraData2D
{
	glm::vec3 position;
	glm::vec2 viewSize;
	float zoom;
};