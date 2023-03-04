#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: Mesh data expression.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include<glm/glm.hpp>
#include<vector>
#include"Core/Data/Vertex.h"
#include<glad.h>
#include <Core/Graphics/Buffer.h>
#include <memory>
#include "Core/Data/UV.h"

#include "BoundingBox.h"

class UV;

enum class DrawType
{
	None,
	Lines,
	Triangles,
};

class BaseMesh
{
public:
	BaseMesh(DrawType draw_type, bool use_index);
	void SetDrawType(DrawType type);
	GLenum GetGLDrawType() const;
	void SetUseIndex(bool use);
	bool GetUseIndex() const;
protected:
	DrawType m_DrawType = DrawType::None;
	bool m_UseIndex = false;
};

class LineMesh : public BaseMesh
{
public:
	LineMesh();
	void AttachBuffer(std::shared_ptr<VertexBuffer> buffer);
	std::shared_ptr<VertexBuffer> GetBuffer() const;
	void SetVertices(std::shared_ptr<std::vector<glm::vec3>> vertices);
	std::shared_ptr<std::vector<glm::vec3>> GetVertices() const;

private:
	std::shared_ptr<VertexBuffer> m_Buffer;
	std::shared_ptr<std::vector<glm::vec3>> m_Vertices;
};

class Mesh : public BaseMesh
{
public:
	Mesh();
	std::shared_ptr<std::vector<Vertex>> GetVertices() const;
	std::shared_ptr<std::vector<unsigned int>> GetIndices() const;

	void SetVertices(std::shared_ptr<std::vector<Vertex>> vertices);
	void SetIndices(std::shared_ptr<std::vector<unsigned int>> indices);
	void SetBoundingBox(std::shared_ptr<BoundingBox> bounding_box);
	void SetUV(std::shared_ptr<UV> uv);
	bool HasUV() const;

	void AttachBuffer(std::shared_ptr<VertexBuffer> buffer);
	void AttachBuffer(std::shared_ptr<IndexBuffer> buffer);

	std::shared_ptr<VertexBuffer> GetBuffer() const;
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
	std::shared_ptr<BoundingBox> GetBoundingBox() const;
	std::shared_ptr<UV> GetUV() const;
private:
	bool m_UseUV;
	std::shared_ptr<std::vector<Vertex>> m_Vertices;
	std::shared_ptr <std::vector<unsigned int>> m_Indices;
	std::shared_ptr<BoundingBox> m_BoundingBox;
	std::shared_ptr<VertexBuffer> m_Buffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	std::shared_ptr<UV> m_UV;
};
