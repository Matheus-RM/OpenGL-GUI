#pragma once

#include "Base/Shape.hpp"
#include "Background.hpp"

namespace Factory::Shape {

	ShapePtr square(const glm::vec2& position, const glm::vec2& size, unsigned int priority,
						int borderRadius, Background::BackgroundData background);

}