#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: MeshGenerator.h
Purpose: helper class for generating meshes.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Core/Data/Mesh.h"
#include <memory>

class MeshGenerator
{
	typedef std::shared_ptr<std::vector<glm::vec3>> PointsPtr;
	typedef std::shared_ptr<std::vector<unsigned int>> IndicesPtr;
public:
	static std::tuple<PointsPtr, IndicesPtr> GeneratePlanePointsWithIndices(float size = 1.f);
	static std::tuple<PointsPtr, IndicesPtr> GenerateSpherePointsWithIndices(float radius = 1.f, int segments = 30, int rings = 30);
	static std::shared_ptr<LineMesh> GenerateOrbit(float radius = 1.f, int numDivisions = 30);

	static std::shared_ptr<Mesh> GenerateFaceNormalMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& face_normals);
	static std::shared_ptr<LineMesh> GenerateFaceNormalLineMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& face_normals, float normal_len = 0.1f);

	static std::shared_ptr<Mesh> GenerateVertexNormalMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& vertex_normals);
	static std::shared_ptr<LineMesh> GenerateVertexNormalLineMesh(const std::vector<glm::vec3>& loaded_points, const std::vector<unsigned int>& loaded_index,
		const std::vector<glm::vec3>& vertex_normals, float normal_len = 0.1f);


	static std::vector<glm::vec3> GenerateFaceNormals(std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_index);
	static std::vector<glm::vec3> GenerateVertexNormals(std::vector<glm::vec3>& loaded_points, std::vector<glm::vec3>& face_normals, 
		std::vector<unsigned int>& loaded_index);

	static std::shared_ptr<BoundingBox> GenerateBoundingBox(std::vector<glm::vec3>& loaded_points);
};