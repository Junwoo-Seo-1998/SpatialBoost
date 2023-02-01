#include "Material.h"

MaterialData& MaterialData::operator=(const bool val)
{
	type = DataType::Bool;
	data.Bool = val;
	return *this;
}

MaterialData& MaterialData::operator=(const int val)
{
	type = DataType::Int;
	data.Int = val;
	return *this;
}

MaterialData& MaterialData::operator=(const float val)
{
	type = DataType::Float;
	data.Float = val;
	return *this;
}

MaterialData& MaterialData::operator=(const glm::vec2& val)
{
	type = DataType::Float2;
	data.Float2 = val;
	return *this;
}

MaterialData& MaterialData::operator=(const glm::vec3& val)
{
	type = DataType::Float3;
	data.Float3 = val;
	return *this;
}

MaterialData& MaterialData::operator=(const glm::vec4& val)
{
	type = DataType::Float4;
	data.Float4 = val;
	return *this;
}

MaterialData& MaterialData::operator=(const glm::mat3& val)
{
	type = DataType::Mat3;
	data.Mat3 = val;
	return *this;
}

MaterialData& MaterialData::operator=(const glm::mat4& val)
{
	type = DataType::Mat4;
	data.Mat4 = val;
	return *this;
}

MaterialData& Material::operator[](const std::string& key)
{
	return data[key];
}
