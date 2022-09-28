#pragma once
#include <string>
#include "Core/Data/Mesh.h"
#include <memory>
#include <unordered_map>

class Shader;

class AssetManager
{
public:
	static std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& file_name, const std::string& key_name);
	static std::shared_ptr<Mesh> GetVertexNormalMesh(const std::string& name);
	static std::shared_ptr<Mesh> GetVertexNormalLineMesh(const std::string& name);
	static std::shared_ptr<Mesh> GetFaceNormalMesh(const std::string& name);
	static std::shared_ptr<Mesh> GetFaceNormalLineMesh(const std::string& name);

	static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& vert_file, const std::string& frag_file);
private:
	static std::unordered_map<std::string, std::shared_ptr<Mesh>>m_VertexNormalMesh;
	static std::unordered_map<std::string, std::shared_ptr<Mesh>>m_VertexNormalLineMesh;
	static std::unordered_map<std::string, std::shared_ptr<Mesh>>m_FaceNormalMesh;
	static std::unordered_map<std::string, std::shared_ptr<Mesh>>m_FaceNormalLineMesh;
};
