#include "Factories/ShapeFactory.hpp"

#include <GL/glew.h>
#include "Factories/GLFactory.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

ShapePtr Factory::Shape::square(const glm::vec2& position, const glm::vec2& size, unsigned int priority,
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

	auto result = ShapePtr(new ShapeData{
		.indices = 6,
		.priority = (float)priority, // pass the given value to the renderer
		.size = size,
		.borderRadius = borderRadius,
		.modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)),
	});

	result->vao = GL::Factory::createVAO();
	result->vbo = GL::Factory::reserveBuffer(GL_ARRAY_BUFFER,
											sizeof(vertices) + background->size() * sizeof(glm::vec4),
											GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), background->size() * sizeof(glm::vec4),
					glm::value_ptr((*background)[0]));

	result->ebo = GL::Factory::createBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
											&indices, GL_STATIC_DRAW);

	GL::Factory::configureVAO(result->vao, {2, 4}, 4);

	return result;
}