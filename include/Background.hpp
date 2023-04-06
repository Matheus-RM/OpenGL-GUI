#pragma once

#include <memory>
#include <array>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

namespace Background {

	using ColorArray = std::array<glm::vec4, 4>;
	using BackgroundData = std::unique_ptr<std::array<glm::vec4, 4>>;

	BackgroundData solid(float r, float g, float b);
	BackgroundData solid(float r, float g, float b, float a);
	BackgroundData solid(const glm::vec4& color);
}