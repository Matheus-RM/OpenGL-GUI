#include "ShaderLoader.hpp"

#include <fstream>
#include <stdexcept>
#include <sstream>

ShaderLoader::ShaderLoader()
{

}

ShaderLoader::~ShaderLoader()
{

}

ShaderLoader& ShaderLoader::getInstance()
{
	static ShaderLoader instance;

	return instance;
}

void ShaderLoader::readShader(const std::string& name, const ShaderConfiguration& configuration)
{
	compileAndLink(configuration);

	Shader newShader = {mNewProgram};

	mShaders.insert(std::make_pair(name, newShader));
}

void ShaderLoader::compileAndLink(const ShaderConfiguration& configuration)
{
	for(const auto& piece : configuration)
		compilePiece(piece);

	linkProgram();
}

void ShaderLoader::compilePiece(const ShaderPiece& piece)
{
	compilePiece(piece.path, piece.type, getShaderFileData(piece.path));
}

void ShaderLoader::compilePiece(const std::string& id, const GLenum& type, const std::string& data)
{
	const auto* shaderData = data.c_str();

	auto&& newShader = glCreateShader(type);
	glShaderSource(newShader, 1, &shaderData, nullptr);
	glCompileShader(newShader);
	
	checkShaderCompilation(newShader, id);

	mCompiledShaders.push_back(newShader);
}


std::string ShaderLoader::getShaderFileData(const std::string& path)
{
	std::ifstream file(path);

	if(!file.is_open())
		throw std::runtime_error("ShaderLoader: can't load file \"" + path + "\".");

	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	return buffer.str();
}

void ShaderLoader::checkShaderCompilation(GLuint shader, const std::string& id)
{
	int status, logLength;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if(!status)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		auto log = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, nullptr, log);

		auto message = "ShaderLoader: could not complete shader \"" + std::string(id) + "\" compilation, GL Message: ";
		message += log;

		delete[] log;

		throw std::runtime_error(message);
	}
}

void ShaderLoader::linkProgram()
{
	mNewProgram = glCreateProgram();

	for(const auto& shader : mCompiledShaders)
		glAttachShader(mNewProgram, shader);

	glLinkProgram(mNewProgram);

	for(const auto& shader : mCompiledShaders)
	{
		glDetachShader(mNewProgram, shader);
		glDeleteShader(shader);
	}

	mCompiledShaders.clear();
}

void ShaderLoader::deleteShader(const std::string& name)
{
	auto&& shaderIt = mShaders.find(name);
	if(shaderIt == mShaders.end())
		return;

	glDeleteProgram(shaderIt->second.program);
}


Shader ShaderLoader::getShader(const std::string& name) const
{
	return mShaders.at(name);
}

Shader ShaderLoader::compileShader(const std::string& vertexShaderData, const std::string& fragmentShaderData)
{
	compilePiece("vertex", GL_VERTEX_SHADER, vertexShaderData);
	compilePiece("fragment", GL_FRAGMENT_SHADER, fragmentShaderData);

	linkProgram();

	return Shader{mNewProgram};
}