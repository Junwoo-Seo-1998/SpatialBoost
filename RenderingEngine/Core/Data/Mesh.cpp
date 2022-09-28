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

void Mesh::SetVertices(std::vector<glm::vec3>& vertices)
{
	m_Vertices = std::move(vertices);
}

void Mesh::SetNormals(std::vector<glm::vec3>& normals)
{
	m_Normals = std::move(normals);
}

void Mesh::SetIndices(std::vector<unsigned>& indices)
{
	m_Indices = std::move(indices);
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

void Mesh::SetUseIndex(bool use)
{
	m_UseIndex = use;
}

bool Mesh::GetUseIndex() const
{
	return m_UseIndex;
}

void Mesh::AttachBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	m_Buffers.push_back(buffer);
}

void Mesh::AttachBuffer(std::shared_ptr<ElementBuffer> buffer)
{
	m_IndexBuffer = buffer;
}

std::vector<std::shared_ptr<VertexBuffer>>& Mesh::GetBuffers()
{
	return m_Buffers;
}

std::shared_ptr<ElementBuffer> Mesh::GetIndexBuffer()
{
	return m_IndexBuffer;
}


