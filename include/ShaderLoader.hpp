#pragma once

#include "Base/Shader.hpp"

#include <vector>
#include <string>
#include <map>
#include <set>

struct ShaderPiece
{
	std::string path;
	GLenum type;
};

using ShaderConfiguration = std::vector<ShaderPiece>;

class ShaderLoader
{
	private:
		ShaderLoader();
		~ShaderLoader();

	public:
		ShaderLoader(const ShaderLoader&) = delete;
		void operator=(const ShaderLoader&) = delete;

	public:
		static ShaderLoader& getInstance();

		void readShader(const std::string& name, const ShaderConfiguration& configuration);
		void deleteShader(const std::string& name);
		
		Shader getShader(const std::string& name) const;

		Shader compileShader(const std::string& vertexShaderData, const std::string& fragmentShaderData);

	private:
		void compileAndLink(const ShaderConfiguration& configuration);

		void compilePiece(const ShaderPiece& piece);
		void compilePiece(const std::string& id, const GLenum& type, const std::string& data);
		
		std::string getShaderFileData(const std::string& path);

		void checkShaderCompilation(GLuint shader, const std::string& id);

		void linkProgram();

	private:

		std::vector<GLuint> mCompiledShaders;
		GLuint mNewProgram;
		std::map<std::string, Shader> mShaders;
};
