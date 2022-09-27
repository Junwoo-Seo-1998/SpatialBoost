#include "Mesh.h"

void Mesh::SetDrawType(DrawType type)
{
	m_DrawType = type;
}

std::vector<glm::vec3>& Mesh::GetVertices()
{
	return m_Vertices;
}

std::vector<glm::vec3>& Mesh::GetNormals()
{
	return m_Normals;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
	return m_Indices;
}

GLenum Mesh::GetGLDrawType() const
{
	switch (m_DrawType)
	{
	case DrawType::Lines: return GL_LINES;
	case DrawType::Triangles: return GL_TRIANGLES;
	}
	return 0;
}
