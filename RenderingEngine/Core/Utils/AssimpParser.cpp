#include "AssimpParser.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void AssimpParser::ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_indices)
{
	Assimp::Importer importer;
	unsigned flags = aiProcess_Triangulate | aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(file_name, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene, scene->mRootNode, loaded_points, loaded_indices);
}

void AssimpParser::ProcessNode(const aiScene* scene, aiNode* node, std::vector<glm::vec3>& loaded_points,
	std::vector<unsigned>& loaded_indices)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		for (unsigned int v = 0; v < mesh->mNumVertices; ++v)
		{
			aiVector3D& verticies = mesh->mVertices[v];
			loaded_points.emplace_back(verticies.x, verticies.y, verticies.z);
		}

		for (unsigned int f = 0; f < mesh->mNumFaces; ++f)
		{
			loaded_indices.emplace_back(mesh->mFaces[f].mIndices[0]);
			loaded_indices.emplace_back(mesh->mFaces[f].mIndices[1]);
			loaded_indices.emplace_back(mesh->mFaces[f].mIndices[2]);
		}
	}

	for (unsigned int i=0; i < node->mNumChildren; ++i)
	{
		ProcessNode(scene, node->mChildren[i], loaded_points, loaded_indices);
	}
}
