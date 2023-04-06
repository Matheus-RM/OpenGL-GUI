#include "Util/BinaryIO.hpp"

void binIO::readText(std::ifstream &file, std::string* result)
{
	unsigned int size;
	file.read(CHAR_PTR(size), sizeof(unsigned int));

	char* array = new char[size+1];

	file.read(array, size * sizeof(char));

	array[size] = '\0';

	result->assign(array);

	delete[] array;
}

void binIO::readTextArray(std::ifstream& file, std::vector<std::string>* result)
{
	unsigned int size;
	readValue(file, &size);

	result->reserve(size);

	for(unsigned int i = 0; i < size; i++)
	{
		auto& text = result->emplace_back();
		readText(file, &text);
	}
}

void binIO::readTextSet(std::ifstream& file, std::set<std::string>* result)
{
	unsigned int size;
	readValue(file, &size);

	for(unsigned int i = 0; i < size; i++)
	{
		std::string text;
		readText(file, &text);

		result->insert(text);
	}
}


void binIO::readVec2(std::ifstream& file, glm::vec2* vec)
{
	readValue(file, &vec->x);
	readValue(file, &vec->y);
}

void binIO::readVec2i(std::ifstream& file, glm::ivec2* vec)
{
	readValue(file, &vec->x);
	readValue(file, &vec->y);
}

void binIO::readVec3(std::ifstream& file, glm::vec3* vec)
{
	readValue(file, &vec->x);
	readValue(file, &vec->y);
	readValue(file, &vec->z);
}

void binIO::readVec3i(std::ifstream& file, glm::ivec3* vec)
{
	readValue(file, &vec->x);
	readValue(file, &vec->y);
	readValue(file, &vec->z);
}


void binIO::jumpText(std::ifstream& file)
{
	jumpArray<char>(file);
}

void binIO::jumpTextArray(std::ifstream& file)
{
	unsigned int size;
	readValue(file, &size);

	for(unsigned int i = 0; i < size; i++)
	{
		jumpText(file);
	}
}
