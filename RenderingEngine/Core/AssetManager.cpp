/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AssetManager.cpp
Purpose: AssetManager for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "AssetManager.h"
#include "Utils/MeshGenerator.h"
#include "Utils/ObjParser.h"
#include "Core/Graphics/Shader.h"
#include <iostream>
#include "Utils/File.h"

std::unordered_map<std::string, std::shared_ptr<Mesh>>AssetManager::m_VertexNormalMesh;
std::unordered_map<std::string, std::shared_ptr<LineMesh>>AssetManager::m_VertexNormalLineMesh;
std::unordered_map<std::string, std::shared_ptr<Mesh>>AssetManager::m_FaceNormalMesh;
std::unordered_map<std::string, std::shared_ptr<LineMesh>>AssetManager::m_FaceNormalLineMesh;
void AssetManager::LoadMeshFromFile(const std::string& file_name, const std::string& key_name)
{
	std::cout << "Load - " << file_name << std::endl;
	std::vector<glm::vec3> loaded_points;
	std::vector<unsigned int> loaded_indices;
	ObjParser::ParseFile(file_name, loaded_points, loaded_indices);

	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(loaded_points, loaded_indices) };
	
	std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(loaded_points, generated_face_normal,loaded_indices) };

	m_FaceNormalMesh[key_name] = MeshGenerator::GenerateFaceNormalMesh(loaded_points, loaded_indices, generated_face_normal);
	m_FaceNormalLineMesh[key_name] = MeshGenerator::GenerateFaceNormalLineMesh(loaded_points, loaded_indices, generated_face_normal);

	m_VertexNormalMesh[key_name] = MeshGenerator::GenerateVertexNormalMesh(loaded_points, loaded_indices, generated_vertex_normal);
	m_VertexNormalLineMesh[key_name] = MeshGenerator::GenerateVertexNormalLineMesh(loaded_points, loaded_indices, generated_vertex_normal);
}

std::shared_ptr<Mesh> AssetManager::GetVertexNormalMesh(const std::string& name)
{
	return m_VertexNormalMesh[name];
}

std::shared_ptr<LineMesh> AssetManager::GetVertexNormalLineMesh(const std::string& name)
{
	return m_VertexNormalLineMesh[name];
}

std::shared_ptr<Mesh> AssetManager::GetFaceNormalMesh(const std::string& name)
{
	return m_FaceNormalMesh[name];
}

std::shared_ptr<LineMesh> AssetManager::GetFaceNormalLineMesh(const std::string& name)
{
	return m_FaceNormalLineMesh[name];
}

void AssetManager::GenerateSphere(const std::string& key_name, float radius, int segments, int rings)
{
	auto [gen_points, gen_indices] = MeshGenerator::GenerateSpherePointsWithIndices(radius, segments, rings);
	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(*gen_points, *gen_indices) };

	std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(*gen_points, generated_face_normal,*gen_indices) };

	m_FaceNormalMesh[key_name] = MeshGenerator::GenerateFaceNormalMesh(*gen_points, *gen_indices, generated_face_normal);
	m_FaceNormalLineMesh[key_name] = MeshGenerator::GenerateFaceNormalLineMesh(*gen_points, *gen_indices, generated_face_normal);

	m_VertexNormalMesh[key_name] = MeshGenerator::GenerateVertexNormalMesh(*gen_points, *gen_indices, generated_vertex_normal);
	m_VertexNormalLineMesh[key_name] = MeshGenerator::GenerateVertexNormalLineMesh(*gen_points, *gen_indices, generated_vertex_normal);
}

std::shared_ptr<Shader> AssetManager::LoadShaderFromFile(const std::string& vert_file, const std::string& frag_file)
{
	return std::make_shared<Shader>(File::ReadFileToString(vert_file), File::ReadFileToString(frag_file));
}

std::shared_ptr<Shader> AssetManager::LoadShaderFromFile(const std::string& common_file, const std::string& vert_file,
	const std::string& frag_file)
{
	return std::make_shared<Shader>(File::ReadFileToString(common_file), File::ReadFileToString(vert_file), File::ReadFileToString(frag_file));
}

