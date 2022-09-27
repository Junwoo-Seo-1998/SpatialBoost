#pragma once
#include <string>
#include "Core/Data/Mesh.h"
#include <memory>

class AssetManager
{
public:
	static std::shared_ptr<Mesh> LoadMeshFromFile(const std::string& file_name);
private:
	
};
