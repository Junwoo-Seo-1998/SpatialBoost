#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Core/UUID.h"
#include "Core/Utils/DataType.h"

struct MaterialData
{
	MaterialData() = default;
	DataType type = DataType::None;

	union Data
	{
		bool Bool;
		int Int;
		float Float;
		glm::vec2 Float2;
		glm::vec3 Float3;
		glm::vec4 Float4;
		glm::mat3 Mat3;
		glm::mat4 Mat4;
	} data;

	MaterialData& operator=(const bool val);
	MaterialData& operator=(const int val);
	MaterialData& operator=(const float val);
	MaterialData& operator=(const glm::vec2& val);
	MaterialData& operator=(const glm::vec3& val);
	MaterialData& operator=(const glm::vec4& val);
	MaterialData& operator=(const glm::mat3& val);
	MaterialData& operator=(const glm::mat4& val);
};

struct Material
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Emissive;
	std::unordered_map<std::string, MaterialData> data;
	MaterialData& operator[](const std::string& key);
};