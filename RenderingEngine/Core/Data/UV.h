#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "Vertex.h"
#include "Core/Graphics/Buffer.h"

struct BoundingBox;

enum class UVTypes
{
	Planar=0,
	CubeMap,
	Spherical,
	Cylindrical,
	Types,
};


class UV
{
public:
	UV(std::vector<glm::vec3>& points, std::vector<glm::vec3>& normals, const BoundingBox& box);
	UV(std::vector<Vertex>& verts, const BoundingBox& box);
	std::shared_ptr<VertexBuffer> GetPointUVBuffer(UVTypes Types);
	std::shared_ptr<VertexBuffer> GetNormalUVBuffer(UVTypes Types);
private:
	void BufferPointUV(UVTypes types, std::vector<glm::vec3>& entry, const BoundingBox& box);
	void BufferNormalUV(UVTypes types, std::vector<glm::vec3>& entry, const BoundingBox& box);

	void BufferPointUV(UVTypes types, std::vector<Vertex>& entry, const BoundingBox& box);
	void BufferNormalUV(UVTypes types, std::vector<Vertex>& entry, const BoundingBox& box);

	std::shared_ptr<VertexBuffer> m_PointUV[static_cast<int>(UVTypes::Types)];
	std::shared_ptr<VertexBuffer> m_NormalUV[static_cast<int>(UVTypes::Types)];
};

