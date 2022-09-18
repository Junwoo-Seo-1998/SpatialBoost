#pragma once
#include<glm/glm.hpp>
#include<string>
#include<glad.h>
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
	Shader() {};
	Shader(const std::string& vertex_src, const std::string& shader_src);
	~Shader();
	void Use() const;
	void SetFloat3(const std::string& name, const glm::vec3& value) const;
	void SetFloat4(const std::string& name, const glm::vec4& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
	GLuint CompileShader(const std::string& src, ShaderFlag flags);
	GLuint m_ShaderProgram;
};