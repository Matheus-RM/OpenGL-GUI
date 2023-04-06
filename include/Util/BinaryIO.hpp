#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define CHAR_PTR(x) ((char*)&x)

namespace binIO
{
	template <typename T>
	void readArray(std::ifstream& file, std::vector<T>* result);

	template <typename T>
	void readValue(std::ifstream& file, T* value);

	void readText(std::ifstream& file, std::string* result);

	void readTextArray(std::ifstream& file, std::vector<std::string>* result);
	void readTextSet(std::ifstream& file, std::set<std::string>* result);
	
	void readVec2(std::ifstream& file, glm::vec2* vec);
	void readVec2i(std::ifstream& file, glm::ivec2* vec);
	void readVec3(std::ifstream& file, glm::vec3* vec);
	void readVec3i(std::ifstream& file, glm::ivec3* vec);

	template <typename T>
	void jumpValue(std::ifstream& file);

	template <typename T>
	void jumpArray(std::ifstream& file);

	void jumpText(std::ifstream& file);
	void jumpTextArray(std::ifstream& file);
	
	template <class T>
	void readArray(std::ifstream& file, std::vector<T>* result)
	{
		unsigned int size;
		readValue(file, &size);

		T* array = new T[size];
		result->reserve(size);

		file.read((char*)array, size * sizeof(T));

		std::move(array, array + size, std::back_inserter(*result));

		delete[] array;
	}

	template <class T>
	void readSet(std::ifstream& file, std::set<T>* result)
	{
		unsigned int size;
		readValue(file, &size);

		T* array = new T[size];
		result->reserve(size);

		file.read((char*)array, size * sizeof(T));

		std::move(array, array + size, std::inserter(*result));

		delete[] array;
	}


	template <typename T>
	void readValue(std::ifstream& file, T* value)
	{
		file.read((char*)value, sizeof(T));
	}

	template <typename T>
	void jumpValue(std::ifstream& file)
	{
		file.seekg(sizeof(T), std::ios::cur);
	}

	template <typename T>
	void jumpArray(std::ifstream& file)
	{
		unsigned int size;
		readValue(file, &size);

		file.seekg(size * sizeof(T), std::ios::cur);
	}
}