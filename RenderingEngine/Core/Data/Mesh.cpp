/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Mesh data expression.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Mesh.h"

BaseMesh::BaseMesh(DrawType draw_type, bool use_index)
	:m_DrawType(draw_type), m_UseIndex(use_index)
{
}

void BaseMesh::SetDrawType(DrawType type)
{
	m_DrawType = type;
}

GLenum BaseMesh::GetGLDrawType() const
{
	switch (m_DrawType)
	{
	case DrawType::Lines: return GL_LINES;
	case DrawType::Triangles: return GL_TRIANGLES;
	}
	return 0;
}

void BaseMesh::SetUseIndex(bool use)
{
	m_UseIndex = use;
}

bool BaseMesh::GetUseIndex() const
{
	return m_UseIndex;
}

LineMesh::LineMesh()
	:BaseMesh(DrawType::Lines, false)
{
}

void LineMesh::AttachBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	m_Buffer = buffer;
}

std::shared_ptr<VertexBuffer> LineMesh::GetBuffer() const
{
	return m_Buffer;
}

void LineMesh::SetVertices(std::shared_ptr<std::vector<glm::vec3>> vertices)
{
	m_Vertices = vertices;
}

std::shared_ptr<std::vector<glm::vec3>> LineMesh::GetVertices() const
{
	return m_Vertices;
}


Mesh::Mesh()
	:BaseMesh(DrawType::Triangles, true)
{
}

std::shared_ptr<std::vector<Vertex>> Mesh::GetVertices() const
{
	return m_Vertices;
}

std::shared_ptr<std::vector<unsigned>> Mesh::GetIndices() const
{
	return m_Indices;
}


void Mesh::SetVertices(std::shared_ptr<std::vector<Vertex>> vertices)
{
	m_Vertices = vertices;
}

void Mesh::SetIndices(std::shared_ptr<std::vector<unsigned>> indices)
{
	m_Indices = indices;
}

void Mesh::AttachBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	m_Buffer = buffer;
}

std::shared_ptr<VertexBuffer> Mesh::GetBuffer() const
{
	return m_Buffer;
}

std::shared_ptr<ElementBuffer> Mesh::GetIndexBuffer() const
{
	return m_IndexBuffer;
}

void Mesh::AttachBuffer(std::shared_ptr<ElementBuffer> buffer)
{
	m_IndexBuffer = buffer;
}
