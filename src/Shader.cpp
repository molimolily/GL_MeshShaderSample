#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::Compile(const GLchar* path, GLenum shaderType)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* shaderCodeChar = shaderCode.c_str();

	// シェーダのコンパイル
	GLuint shader;
	int success;
	char infoLog[512];
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCodeChar, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// シェーダプログラムのアタッチ
	glAttachShader(this->program, shader);

	// シェーダオブジェクトの削除
	glDeleteShader(shader);
}

void Shader::Link()
{
	int success;
	char infoLog[512];

	// シェーダプログラムのリンク
	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

Shader::Shader(const GLchar* path, const ShaderType type) : program(glCreateProgram())
{
	Compile(path, ShaderTypeToGLenum(type));
	Link();
}

Shader::Shader(const GLchar* path1, const GLchar* path2, const ShaderType type1, const ShaderType type2) : program(glCreateProgram())
{
	Compile(path1, ShaderTypeToGLenum(type1));
	Compile(path2, ShaderTypeToGLenum(type2));
	Link();
}

Shader::Shader(const GLchar* path1, const GLchar* path2, const GLchar* path3, const ShaderType type1, const ShaderType type2, const ShaderType type3) : program(glCreateProgram())
{
	Compile(path1, ShaderTypeToGLenum(type1));
	Compile(path2, ShaderTypeToGLenum(type2));
	Compile(path3, ShaderTypeToGLenum(type3));
	Link();
}

Shader::~Shader()
{
	glDeleteProgram(this->program);
}

void Shader::Use()
{
	glUseProgram(this->program);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->program, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->program, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->program, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(this->program, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(this->program, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(this->program, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

GLuint Shader::GetProgram() const
{
	return program;
}

GLenum Shader::ShaderTypeToGLenum(ShaderType type)
{
	switch (type)
	{
	case ShaderType::VERTEX:
		return GL_VERTEX_SHADER;
	case ShaderType::FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case ShaderType::COMPUTE:
		return GL_COMPUTE_SHADER;
	case ShaderType::MESH:
		return GL_MESH_SHADER_NV;
	case ShaderType::TASK:
		return GL_TASK_SHADER_NV;
	default:
		std::cerr << "ERROR::SHADER::TYPE_NOT_FOUND" << std::endl;
		return GL_VERTEX_SHADER;
	}
}
