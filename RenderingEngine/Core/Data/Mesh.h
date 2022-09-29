#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"Core/Data/Vertex.h"
#include"Core/Data/Face.h"
#include<glad.h>
#include <Core/Graphics/Buffer.h>
#include <memory>
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

	void AttachBuffer(std::shared_ptr<VertexBuffer> buffer);
	void AttachBuffer(std::shared_ptr<ElementBuffer> buffer);

	std::shared_ptr<VertexBuffer> GetBuffer() const;
	std::shared_ptr<ElementBuffer> GetIndexBuffer() const;
private:
	std::shared_ptr<std::vector<Vertex>> m_Vertices;
	std::shared_ptr <std::vector<unsigned int>> m_Indices;

	std::shared_ptr<VertexBuffer> m_Buffer;
	std::shared_ptr<ElementBuffer> m_IndexBuffer;
};
