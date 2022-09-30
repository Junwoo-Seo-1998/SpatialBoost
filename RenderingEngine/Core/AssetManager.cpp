#include "AssetManager.h"

#include "Utils/MeshGenerator.h"
#include "Utils/ObjParser.h"
#include "Core/Graphics/Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
std::unordered_map<std::string, std::shared_ptr<Mesh>>AssetManager::m_VertexNormalMesh;
std::unordered_map<std::string, std::shared_ptr<LineMesh>>AssetManager::m_VertexNormalLineMesh;
std::unordered_map<std::string, std::shared_ptr<Mesh>>AssetManager::m_FaceNormalMesh;
std::unordered_map<std::string, std::shared_ptr<LineMesh>>AssetManager::m_FaceNormalLineMesh;
void AssetManager::LoadMeshFromFile(const std::string& file_name, const std::string& key_name)
{
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
	
	std::ifstream vert_src_file(vert_file);
	std::ifstream frag_src_file(frag_file);
	std::stringstream ss_vert, ss_frag;
	ss_vert << vert_src_file.rdbuf();
	ss_frag << frag_src_file.rdbuf();

	return std::make_shared<Shader>(ss_vert.str(), ss_frag.str());
}
