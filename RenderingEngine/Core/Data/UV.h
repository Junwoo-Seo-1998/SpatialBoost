#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "Vertex.h"
#include "Core/Graphics/Buffer.h"

struct BoundingBox;

enum class UVTypes
{
	Cylindrical=0,
	Spherical,
	CubeMap,
	Planar,
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

