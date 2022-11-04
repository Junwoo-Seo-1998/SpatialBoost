#include "UV.h"

#include "BoundingBox.h"
#include "Core/Utils/Math.h"
UV::UV(std::vector<glm::vec3>& points, std::vector<glm::vec3>& normals, const BoundingBox& box)
{
	BufferPointUV(UVTypes::Planar, points, box);
	BufferPointUV(UVTypes::CubeMap, points, box);
	BufferPointUV(UVTypes::Cylindrical, points, box);
	BufferPointUV(UVTypes::Spherical, points, box);

	BufferNormalUV(UVTypes::Planar, normals, box);
	BufferNormalUV(UVTypes::CubeMap, normals, box);
	BufferNormalUV(UVTypes::Cylindrical, normals, box);
	BufferNormalUV(UVTypes::Spherical, normals, box);
}

UV::UV(std::vector<Vertex>& verts, const BoundingBox& box)
{
	BufferPointUV(UVTypes::Planar, verts, box);
	BufferPointUV(UVTypes::CubeMap, verts, box);
	BufferPointUV(UVTypes::Cylindrical, verts, box);
	BufferPointUV(UVTypes::Spherical, verts, box);

	BufferNormalUV(UVTypes::Planar, verts, box);
	BufferNormalUV(UVTypes::CubeMap, verts, box);
	BufferNormalUV(UVTypes::Cylindrical, verts, box);
	BufferNormalUV(UVTypes::Spherical, verts, box);
}

std::shared_ptr<VertexBuffer> UV::GetPointUVBuffer(UVTypes Types)
{
	return m_PointUV[static_cast<int>(Types)];
}

std::shared_ptr<VertexBuffer> UV::GetNormalUVBuffer(UVTypes Types)
{
	return m_NormalUV[static_cast<int>(Types)];
}

void UV::BufferPointUV(UVTypes types, std::vector<glm::vec3>& entry, const BoundingBox& box)
{
	std::shared_ptr< std::vector<glm::vec2>> uv;
	switch (types)
	{
	case UVTypes::Planar:
		uv = Math::ComputePlanarUVs(entry, box);
		break;
	case UVTypes::CubeMap:
		uv = Math::ComputeCubeMapUVs(entry);
		break;
	case UVTypes::Spherical:
		uv = Math::ComputeSphericalUVs(entry);
		break;
	case UVTypes::Cylindrical:
		uv = Math::ComputeCylindricalUVs(entry, box);
		break;
	}

	m_PointUV[static_cast<int>(types)] = std::make_shared<VertexBuffer>(uv->data(), uv->size() * sizeof(glm::vec2));
	m_PointUV[static_cast<int>(types)]->DescribeData({ {2,Float2} });
}

void UV::BufferNormalUV(UVTypes types, std::vector<glm::vec3>& entry, const BoundingBox& box)
{
	std::shared_ptr< std::vector<glm::vec2>> uv;
	switch (types)
	{
	case UVTypes::Planar:
		uv = Math::ComputePlanarUVs(entry, box);
		break;
	case UVTypes::CubeMap:
		uv = Math::ComputeCubeMapUVs(entry);
		break;
	case UVTypes::Spherical:
		uv = Math::ComputeSphericalUVs(entry);
		break;
	case UVTypes::Cylindrical:
		uv = Math::ComputeCylindricalUVs(entry, box);
		break;
	}

	m_NormalUV[static_cast<int>(types)] = std::make_shared<VertexBuffer>(uv->data(), uv->size() * sizeof(glm::vec2));
	m_NormalUV[static_cast<int>(types)]->DescribeData({ {2,Float2} });
}

void UV::BufferPointUV(UVTypes types, std::vector<Vertex>& entry, const BoundingBox& box)
{
	std::shared_ptr< std::vector<glm::vec2>> uvs = std::make_shared<std::vector<glm::vec2>>();
	glm::vec3 min = box.min - box.center;
	glm::vec3 max = box.max - box.center;
	for (auto& vert : entry)
	{
		glm::vec2 uv;
		switch (types)
		{

		case UVTypes::Planar:
			uv = Math::ComputePlanarUV(vert.position - box.center, min.x, max.x, min.y, max.y);
			break;
		case UVTypes::CubeMap:
			uv = Math::ComputeCubeMapUV(vert.position - box.center);
			break;
		case UVTypes::Spherical:
			uv = Math::ComputeSphericalUV(vert.position - box.center);
			break;
		case UVTypes::Cylindrical:
			uv = Math::ComputeCylindricalUV(vert.position - box.center, min.y, max.y);
			break;
		}
		uvs->push_back(uv);
	}
	m_PointUV[static_cast<int>(types)] = std::make_shared<VertexBuffer>(uvs->data(), uvs->size() * sizeof(glm::vec2));
	m_PointUV[static_cast<int>(types)]->DescribeData({ {2,Float2} });
}


void UV::BufferNormalUV(UVTypes types, std::vector<Vertex>& entry, const BoundingBox& box)
{
	std::shared_ptr< std::vector<glm::vec2>> uvs=std::make_shared<std::vector<glm::vec2>>();
	glm::vec3 min = box.min - box.center;
	glm::vec3 max = box.max - box.center;
	for (auto& vert : entry)
	{
		glm::vec2 uv;
		switch (types)
		{

		case UVTypes::Planar:
			uv = Math::ComputePlanarUV(vert.normal, min.x, max.x, min.y, max.y);
			break;
		case UVTypes::CubeMap:
			uv = Math::ComputeCubeMapUV(vert.normal);
			break;
		case UVTypes::Spherical:
			uv = Math::ComputeSphericalUV(vert.normal);
			break;
		case UVTypes::Cylindrical:
			uv = Math::ComputeCylindricalUV(vert.normal, min.y, max.y);
			break;
		}
		uvs->push_back(uv);
	}
	m_NormalUV[static_cast<int>(types)] = std::make_shared<VertexBuffer>(uvs->data(), uvs->size() * sizeof(glm::vec2));
	m_NormalUV[static_cast<int>(types)]->DescribeData({ {2,Float2} });
}
