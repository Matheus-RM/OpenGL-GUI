#include "Background.hpp"


Background::BackgroundData Background::solid(float r, float g, float b)
{
	return solid(glm::vec4(r, g, b, 1.0f));
}

Background::BackgroundData Background::solid(float r, float g, float b, float a)
{
	return solid(glm::vec4(r, g, b, a));
}

Background::BackgroundData Background::solid(const glm::vec4& color)
{
	auto result = std::make_unique<Background::ColorArray>();
	result->fill(color);

	return result;
}
