#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/matrix_float4x4.hpp>

struct Shape
{
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	unsigned int indices;

	unsigned int id;
	float priority;
	glm::ivec2 size;
	int borderRadius;

	glm::mat4 modelMatrix;
};