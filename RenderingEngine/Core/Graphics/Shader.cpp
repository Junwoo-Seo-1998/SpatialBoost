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
Shader::Shader(const std::string& vertex_src, const std::string& fragment_src)
	:common(""), vertex_src(vertex_src), fragment_src(fragment_src)
{
	GLuint vert = CompileShader(vertex_src, ShaderFlag::VertexShader);
	GLuint frag = CompileShader(fragment_src, ShaderFlag::FragmentShader);
	
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vert);
	glAttachShader(m_ShaderProgram, frag);
	glLinkProgram(m_ShaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		assert(true, "ERROR::SHADER::PROGRAM::LINK_FAILED\n");
	}


	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::Shader(const std::string& common, const std::string& vertex_src,
	const std::string& fragment_src)
	:common(common), vertex_src(vertex_src), fragment_src(fragment_src)
{
	GLuint vert = CompileShader(common, vertex_src, ShaderFlag::VertexShader);
	GLuint frag = CompileShader(common, fragment_src, ShaderFlag::FragmentShader);

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vert);
	glAttachShader(m_ShaderProgram, frag);
	glLinkProgram(m_ShaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		assert(true, "ERROR::SHADER::PROGRAM::LINK_FAILED\n");
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Use() const
{
	glUseProgram(m_ShaderProgram);
}

void Shader::SetInt(const std::string& name, const int value) const
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	glUniform1f(location, value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value) const
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value) const
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned unit)
{
	Use();
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "error on shader location! - " << name << std::endl;
		return;
	}
	texture->BindTexture(unit);
	glUniform1i(location, unit);
}

void Shader::Reload()
{
	glUseProgram(0);
	glDeleteProgram(m_ShaderProgram);

	GLuint vert = CompileShader(common, vertex_src, ShaderFlag::VertexShader);
	GLuint frag = CompileShader(common, fragment_src, ShaderFlag::FragmentShader);

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vert);
	glAttachShader(m_ShaderProgram, frag);
	glLinkProgram(m_ShaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		assert(true, "ERROR::SHADER::PROGRAM::LINK_FAILED\n");
	}

	glDeleteShader(vert);
	glDeleteShader(frag);
}

GLuint Shader::GetProgramID()
{
	return m_ShaderProgram;
}

GLuint Shader::CompileShader(const std::string& src, ShaderFlag flags)
{
	GLenum type = GL_NONE;
	switch (flags)
	{
	case ShaderFlag::VertexShader:
		type = GL_VERTEX_SHADER;
		break;
	case ShaderFlag::FragmentShader:
		type = GL_FRAGMENT_SHADER;
		break;
	}

	GLuint shader = glCreateShader(type);
	const char* const_src_str = src.c_str();
	glShaderSource(shader, 1, &const_src_str, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		assert(true, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	return shader;
}

GLuint Shader::CompileShader(const std::string& common, const std::string& src, ShaderFlag flags)
{
	GLenum type = GL_NONE;
	switch (flags)
	{
	case ShaderFlag::VertexShader:
		type = GL_VERTEX_SHADER;
		break;
	case ShaderFlag::FragmentShader:
		type = GL_FRAGMENT_SHADER;
		break;
	}

	GLuint shader = glCreateShader(type);
	const char* src_array[] = { common.c_str(), src.c_str() };
	int lengths[] = { static_cast<int>(common.size()),  static_cast<int>(src.size()) };
	glShaderSource(shader, 2, src_array, lengths);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		assert(true, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
	}
	return shader;
}