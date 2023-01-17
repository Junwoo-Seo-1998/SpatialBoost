#pragma once
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
#include<glm/glm.hpp>
#include<string>
#include<glad.h>
#include <memory>

#include "FrameBuffer.h"
class Texture;

enum class ShaderFlag
{
	None = 0 ,
	VertexShader,
	FragmentShader,
	//TessellationShader,
	//GeometryShader,
};


class Shader
{
public:
	Shader() :m_ShaderProgram(0) {}
	Shader(const std::string& vertex_src, const std::string& fragment_src);
	Shader(const std::string& common, const std::string& vertex_src, const std::string& fragment_src);
	~Shader();
	void Use() const;
	void SetInt(const std::string& name, const int value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetFloat3(const std::string& name, const glm::vec3& value) const;
	void SetFloat4(const std::string& name, const glm::vec4& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

	void SetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit = 0);
	void Reload();

	GLuint GetProgramID();
private:
	GLuint CompileShader(const std::string& src, ShaderFlag flags);
	GLuint CompileShader(const std::string& common, const std::string& src, ShaderFlag flags);
	GLuint m_ShaderProgram;
	const std::string common;
	const std::string vertex_src;
	const std::string fragment_src;
};


