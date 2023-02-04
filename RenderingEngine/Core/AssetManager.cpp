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

#include "UUID.h"
#include "Utils/File.h"
#include "Core/Data/Texture.h"
#include "Core/Data/TextureData.h"
#include "Data/UV.h"
std::shared_ptr<VertexBuffer>AssetManager::m_Skybox;
std::unordered_map<std::string, UUID> m_UUIDMap;

std::unordered_map<UUID, std::shared_ptr<Mesh>>AssetManager::m_VertexNormalMesh;
std::unordered_map<UUID, std::shared_ptr<LineMesh>>AssetManager::m_VertexNormalLineMesh;
std::unordered_map<UUID, std::shared_ptr<Mesh>>AssetManager::m_FaceNormalMesh;
std::unordered_map<UUID, std::shared_ptr<LineMesh>>AssetManager::m_FaceNormalLineMesh;

std::unordered_map<UUID, std::shared_ptr<Shader>>AssetManager::m_Shaders;
std::unordered_map<std::string, std::shared_ptr<Texture>>AssetManager::m_Textures;

std::unordered_map<std::string, std::shared_ptr<Scene>>AssetManager::m_Scenes;

UUID AssetManager::GetUUID(const std::string& asset_name)
{
	return m_UUIDMap[asset_name];
}

void AssetManager::LoadMeshFromFile(const std::string& file_name, const std::string& key_name)
{
	std::cout << "Load - " << file_name << std::endl;
	std::vector<glm::vec3> loaded_points;
	std::vector<unsigned int> loaded_indices;
	ObjParser::ParseFile(file_name, loaded_points, loaded_indices);

	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(loaded_points, loaded_indices) };
	
	std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(loaded_points, generated_face_normal,loaded_indices) };

	std::shared_ptr<BoundingBox> box = MeshGenerator::GenerateBoundingBox(loaded_points);

	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_FaceNormalMesh[uuid] = MeshGenerator::GenerateFaceNormalMesh(loaded_points, loaded_indices, generated_face_normal);
	m_FaceNormalMesh[uuid]->SetBoundingBox(box);

	m_FaceNormalLineMesh[uuid] = MeshGenerator::GenerateFaceNormalLineMesh(loaded_points, loaded_indices, generated_face_normal);
	

	m_VertexNormalMesh[uuid] = MeshGenerator::GenerateVertexNormalMesh(loaded_points, loaded_indices, generated_vertex_normal);
	m_VertexNormalMesh[uuid]->SetBoundingBox(box);
	m_VertexNormalMesh[uuid]->SetUV(std::make_shared<UV>(*m_VertexNormalMesh[uuid]->GetVertices(), *box));

	m_VertexNormalLineMesh[uuid] = MeshGenerator::GenerateVertexNormalLineMesh(loaded_points, loaded_indices, generated_vertex_normal);
}

std::shared_ptr<Mesh> AssetManager::GetVertexNormalMesh(UUID uuid)
{
	return m_VertexNormalMesh[uuid];
}

std::shared_ptr<LineMesh> AssetManager::GetVertexNormalLineMesh(UUID uuid)
{
	return m_VertexNormalLineMesh[uuid];
}

std::shared_ptr<Mesh> AssetManager::GetFaceNormalMesh(UUID uuid)
{
	return m_FaceNormalMesh[uuid];
}

std::shared_ptr<LineMesh> AssetManager::GetFaceNormalLineMesh(UUID uuid)
{
	return m_FaceNormalLineMesh[uuid];
}

void AssetManager::GeneratePlane(const std::string& key_name, float size)
{
	auto [gen_points, gen_indices] = MeshGenerator::GeneratePlanePointsWithIndices(size);
	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(*gen_points, *gen_indices) };

	std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(*gen_points, generated_face_normal,*gen_indices) };

	std::shared_ptr<BoundingBox> box = MeshGenerator::GenerateBoundingBox(*gen_points);

	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_FaceNormalMesh[uuid] = MeshGenerator::GenerateFaceNormalMesh(*gen_points, *gen_indices, generated_face_normal);
	m_FaceNormalMesh[uuid]->SetBoundingBox(box);
	m_FaceNormalLineMesh[uuid] = MeshGenerator::GenerateFaceNormalLineMesh(*gen_points, *gen_indices, generated_face_normal);

	m_VertexNormalMesh[uuid] = MeshGenerator::GenerateVertexNormalMesh(*gen_points, *gen_indices, generated_vertex_normal);
	m_VertexNormalMesh[uuid]->SetBoundingBox(box);
	m_VertexNormalLineMesh[uuid] = MeshGenerator::GenerateVertexNormalLineMesh(*gen_points, *gen_indices, generated_vertex_normal);
	m_VertexNormalMesh[uuid]->SetUV(std::make_shared<UV>(*m_VertexNormalMesh[uuid]->GetVertices(), *box));
}

void AssetManager::GenerateSphere(const std::string& key_name, float radius, int segments, int rings)
{
	auto [gen_points, gen_indices] = MeshGenerator::GenerateSpherePointsWithIndices(radius, segments, rings);
	std::vector<glm::vec3> generated_face_normal{ MeshGenerator::GenerateFaceNormals(*gen_points, *gen_indices) };

	std::vector<glm::vec3> generated_vertex_normal{ MeshGenerator::GenerateVertexNormals(*gen_points, generated_face_normal,*gen_indices) };

	std::shared_ptr<BoundingBox> box = MeshGenerator::GenerateBoundingBox(*gen_points);

	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_FaceNormalMesh[uuid] = MeshGenerator::GenerateFaceNormalMesh(*gen_points, *gen_indices, generated_face_normal);
	m_FaceNormalMesh[uuid]->SetBoundingBox(box);
	m_FaceNormalLineMesh[uuid] = MeshGenerator::GenerateFaceNormalLineMesh(*gen_points, *gen_indices, generated_face_normal);

	m_VertexNormalMesh[uuid] = MeshGenerator::GenerateVertexNormalMesh(*gen_points, *gen_indices, generated_vertex_normal);
	m_VertexNormalMesh[uuid]->SetBoundingBox(box);
	m_VertexNormalMesh[uuid]->SetUV(std::make_shared<UV>(*m_VertexNormalMesh[uuid]->GetVertices(), *box));

	m_VertexNormalLineMesh[uuid] = MeshGenerator::GenerateVertexNormalLineMesh(*gen_points, *gen_indices, generated_vertex_normal);
}

void AssetManager::GenerateSkybox()
{
	constexpr float skyboxVertices[6][18] = {
		{//front
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		},
		{//left
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		},
		{//right
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 },
		{//back
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		},
		{//top
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		},
		{//bottom
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
		 }
	};
	m_Skybox = VertexBuffer::CreateVertexBuffer(sizeof(skyboxVertices));
	m_Skybox->SetData(sizeof(skyboxVertices), skyboxVertices);
	m_Skybox->SetDataTypes({ {0, DataType::Float3} });
}

std::shared_ptr<VertexBuffer> AssetManager::GetSkybox()
{
	return m_Skybox;
}

std::shared_ptr<Shader> AssetManager::LoadShaderFromFile(const std::string& key_name, const std::string& vert_file, const std::string& frag_file)
{
	std::cout << "Shader Files: " << vert_file << ", " << frag_file << std::endl;
	std::shared_ptr<Shader> shader = Shader::CreateShaderFromFile({ 
		{ShaderType::VertexShader,{vert_file}}, {ShaderType::FragmentShader,{frag_file}}
	});
	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_Shaders[uuid] = shader;
	return shader;
}

std::shared_ptr<Shader> AssetManager::LoadShaderFromFile(const std::string& key_name, const std::string& common_file, const std::string& vert_file,
	const std::string& frag_file)
{
	std::cout << "Shader Files: " << common_file << ", " << vert_file << ", " << frag_file << std::endl;
	std::shared_ptr<Shader> shader = Shader::CreateShaderFromFile({
		{ShaderType::VertexShader,{common_file,vert_file}}, {ShaderType::FragmentShader,{common_file,frag_file}}
		});
	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_Shaders[uuid] = shader;
	return shader;
}

std::shared_ptr<Shader> AssetManager::ReloadShaderFromFile(const std::string& key_name, const std::string& common_file,
	const std::string& vert_file, const std::string& frag_file)
{
	std::cout << "Reload Shader Files: " << common_file << ", " << vert_file << ", " << frag_file << std::endl;
	std::shared_ptr<Shader> shader = Shader::CreateShaderFromFile({
		{ShaderType::VertexShader,{common_file,vert_file}}, {ShaderType::FragmentShader,{common_file,frag_file}}
		});
	UUID uuid{};
	m_UUIDMap[key_name] = uuid;

	m_Shaders[uuid] = shader;
	return shader;
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& key_name)
{
	return m_Shaders[m_UUIDMap[key_name]];
}

std::shared_ptr<Shader> AssetManager::GetShader(UUID uuid)
{
	return m_Shaders[uuid];
}

std::shared_ptr<Texture> AssetManager::LoadTextureFromFile(const std::string& key_name, const std::string& file_name)
{
	std::shared_ptr<Texture> texture = Texture::CreateTexture(File::ReadImageToTexture(file_name));
	m_Textures[key_name] = texture;
	return texture;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& key_name)
{
	return m_Textures[key_name];
}

void AssetManager::RegisterScene(const std::string& scene_name, std::shared_ptr<Scene> scene)
{
	m_Scenes[scene_name] = scene;
}

std::shared_ptr<Scene> AssetManager::GetScene(const std::string& scene_name)
{
	return m_Scenes[scene_name];
}

