/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.cpp
Purpose: Abstracted class for shader program
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 10 2022
End Header --------------------------------------------------------*/
#include"Shader.h"
#include<iostream>
#include "glm/gtc/type_ptr.hpp"
#include "Core/Data/Texture.h"
#include "Core/Utils/File.h"
#include "Core/Data/Material.h"
Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(m_ShaderProgram);
}

std::shared_ptr<Shader> Shader::CreateShaderFromString(const ShaderSource& srcs)
{
	return std::shared_ptr<Shader>(new Shader{ srcs, false });
}

std::shared_ptr<Shader> Shader::CreateShaderFromFile(const ShaderSource& srcs)
{
	return std::shared_ptr<Shader>(new Shader{ srcs, true });
}


void Shader::Use() const
{
	glUseProgram(m_ShaderProgram);
}

void Shader::SetInt(const std::string& name, const int value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	Use();
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetCustomMaterial(const std::unordered_map<std::string, MaterialData>& Data) const
{
	for (auto& pair : Data)
	{
		switch (pair.second.type)
		{
		case DataType::Bool:
			SetInt(pair.first, pair.second.data.Bool);
			break;
		case DataType::Int:
			SetInt(pair.first, pair.second.data.Int);
			break;
		case DataType::Float:
			SetFloat(pair.first, pair.second.data.Float);
			break;
		case DataType::Float3:
			SetFloat3(pair.first, pair.second.data.Float3);
			break;
		case DataType::Float4:
			SetFloat4(pair.first, pair.second.data.Float4);
			break;
		case DataType::Mat4:
			SetMat4(pair.first, pair.second.data.Mat4);
			break;
		default:
			break;
		}
	}
}

void Shader::SetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned unit) const
{
	Use();
	int location = GetUniformLocation(name);
	texture->BindTexture(unit);
	glUniform1i(location, unit);
}

unsigned Shader::GetShaderProgram()
{
	return m_ShaderProgram;
}

Shader::Shader()
	:m_ShaderProgram(0)
{

}

Shader::Shader(const ShaderSource& shaderSrc, bool is_file)
	:m_ShaderProgram(0)
{
	m_ShaderProgram = glCreateProgram();

	std::vector<unsigned> shaders;
	for (auto& [type, src] : shaderSrc)
	{
		unsigned shader = 0;
		if (is_file)
		{
			std::vector<std::string> loaded_strings;
			for (const auto& filePath : src)
			{
				loaded_strings.push_back(File::ReadFileToString(filePath));
			}
			shader = CompileShader(type, loaded_strings);
		}
		else
		{
			shader = CompileShader(type, src);
		}
		glAttachShader(m_ShaderProgram, shader);
		shaders.push_back(shader);
	}

	glLinkProgram(m_ShaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	//no need only program
	for (auto shader : shaders)
		glDeleteShader(shader);

	/*GLint numActiveUniforms = 0;
	glGetProgramiv(m_ShaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	printf("Active Uniforms: %d\n", numActiveUniforms);

	for (int i = 0; i < numActiveUniforms; ++i)
	{
		char buffer[256] = { 0, };
		GLsizei len = 0;
		GLint size = 0;
		GLenum type;
		glGetActiveUniform(m_ShaderProgram, (GLuint)i, 255, &len, &size, &type, buffer);

		printf("Uniform #%d Type: %u Name: %s\n", i, type, buffer);
	}*/
}


int Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return -1;
	}
	return location;
}

int Shader::TryUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	return location;
}

void Shader::TrySetInt(const std::string& name, const int value) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	glUniform1i(location, value);
}

void Shader::TrySetFloat(const std::string& name, const float value) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	glUniform1f(location, value);
}

void Shader::TrySetFloat3(const std::string& name, const glm::vec3& value) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::TrySetFloat4(const std::string& name, const glm::vec4& value) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::TrySetMat4(const std::string& name, const glm::mat4& value) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::TrySetMaterial(const std::string& name, const Material& mat) const
{
	
	TrySetTexture(name +".Diffuse", mat.Diffuse, 6);
	TrySetTexture(name + ".Specular", mat.Specular, 7);
	TrySetTexture(name + ".Emissive", mat.Emissive, 8);
	TrySetFloat(name + ".Shininess", mat.Shininess);
}

void Shader::TrySetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned unit) const
{
	int location = TryUniformLocation(name);
	if (location == -1)
		return;
	texture->BindTexture(unit);
	glUniform1i(location, unit);
}

unsigned Shader::CompileShader(ShaderType type, const std::vector<std::string>& src)
{
	GLenum glsl_type = GL_NONE;
	switch (type)
	{
	case ShaderType::VertexShader:
		glsl_type = GL_VERTEX_SHADER;
		break;
	case ShaderType::FragmentShader:
		glsl_type = GL_FRAGMENT_SHADER;
		break;
	}

	GLuint shader = glCreateShader(glsl_type);
	const int src_count = static_cast<int>(src.size());
	std::vector<const char*> src_array;
	std::vector<int> src_len;

	for (auto& src_string : src)
	{
		src_array.push_back(src_string.c_str());
		src_len.push_back(static_cast<int>(src_string.length()));
	}

	glShaderSource(shader, src_count, src_array.data(), src_len.data());
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		static_assert(true, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	return shader;
}
