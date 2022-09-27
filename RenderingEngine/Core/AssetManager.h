#pragma once
#include <string>
#include "Core/Data/Mesh.h"


class AssetManager
{
public:
	static Mesh LoadMeshFromFile(const std::string& file_name);
private:

};
