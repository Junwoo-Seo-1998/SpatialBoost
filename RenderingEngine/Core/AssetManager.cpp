#include "AssetManager.h"

#include "Utils/MeshGenerator.h"
#include "Utils/ObjParser.h"

std::shared_ptr<Mesh> AssetManager::LoadMeshFromFile(const std::string& file_name)
{
	std::vector<glm::vec3> loaded_points;
	std::vector<unsigned int> loaded_indices;
	ObjParser::ParseFile(file_name, loaded_points, loaded_indices);

	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(loaded_points, loaded_indices) };
	
	//std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(loaded_points, generated_face_normal,loaded_indices) };

	return  MeshGenerator::GenerateFaceNormalMesh(loaded_points, loaded_indices, generated_face_normal);


}
