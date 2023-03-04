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
#include <unordered_map>

#include "FrameBuffer.h"
class DebugRenderer;
struct MaterialData;
struct Material;

enum class ShaderType
{
	VertexShader = 0,
	FragmentShader,
	//TessellationShader,
	//GeometryShader,
	TypeCounts,
};


using ShaderSource = std::unordered_map<ShaderType, std::vector<std::string>>;

class RenderLayer;
class Shader
{
	friend RenderLayer;
	friend DebugRenderer;
public:
	~Shader();
	static std::shared_ptr<Shader> CreateShaderFromString(const ShaderSource& srcs);
	static std::shared_ptr<Shader> CreateShaderFromFile(const ShaderSource& srcs);

	void Use() const;
	void SetInt(const std::string& name, const int value) const;
	void SetFloat(const std::string& name, const float value) const;
	void SetFloat3(const std::string& name, const glm::vec3& value) const;
	void SetFloat4(const std::string& name, const glm::vec4& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetCustomMaterial(const std::unordered_map<std::string, MaterialData>& Data) const;
	void SetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit = 0) const;
	unsigned GetShaderProgram();
private:
	Shader();
	Shader(const ShaderSource& shaderSrc, bool is_file);

	int GetUniformLocation(const std::string& name) const;
	int TryUniformLocation(const std::string& name) const;

	void TrySetInt(const std::string& name, const int value) const;
	void TrySetFloat(const std::string& name, const float value) const;
	void TrySetFloat3(const std::string& name, const glm::vec3& value) const;
	void TrySetFloat4(const std::string& name, const glm::vec4& value) const;
	void TrySetMat4(const std::string& name, const glm::mat4& value) const;
	void TrySetMaterial(const std::string& name, const Material& mat) const;
	void TrySetTexture(const std::string& name, std::shared_ptr<Texture> texture, unsigned int unit = 0) const;
	unsigned CompileShader(ShaderType type, const std::vector<std::string>& src);
	unsigned m_ShaderProgram;
};


