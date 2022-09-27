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


class Mesh
{
public:
	void SetDrawType(DrawType type);
	std::vector<glm::vec3>& GetVertices();
	std::vector<glm::vec3>& GetNormals();
	std::vector<unsigned int>& GetIndices();

	void SetVertices(std::vector<glm::vec3>& vertices);
	void SetNormals(std::vector<glm::vec3>& normals);
	void SetIndices(std::vector<unsigned int>& indices);
	
	GLenum GetGLDrawType() const;
	void SetUseIndex(bool use);

	void AttachBuffer(std::shared_ptr<VertexBuffer> buffer);
	void AttachBuffer(std::shared_ptr<ElementBuffer> buffer);

	std::vector<std::shared_ptr<VertexBuffer>>& GetBuffers();
	std::shared_ptr<ElementBuffer> GetIndexBuffer();
private:
	DrawType m_DrawType = DrawType::None;
	bool m_UseIndex = false;
	std::vector<glm::vec3> m_Vertices;
	std::vector<glm::vec3> m_Normals;
	std::vector<unsigned int> m_Indices;

	std::vector<std::shared_ptr<VertexBuffer>> m_Buffers;
	std::shared_ptr<ElementBuffer> m_IndexBuffer;
};
