#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"Core/Data/Vertex.h"
#include"Core/Data/Face.h"
#include<glad.h>
enum class DrawType
{
	None,
	Lines,
	Triangles,
};


class Mesh
{
public:
	void SetDrawType(DrawType type);
	std::vector<glm::vec3>& GetVertices();
	std::vector<glm::vec3>& GetNormals();
	std::vector<unsigned int>& GetIndices();

	GLenum GetGLDrawType() const;
private:
	DrawType m_DrawType = DrawType::None;
	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned int> m_Indices;
};
