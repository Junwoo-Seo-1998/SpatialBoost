#pragma once
#include <string>
#include"Core/Data/Mesh.h"
class ObjParser
{
public:
	Mesh LoadFile(const std::string& file_name);
	Mesh LoadFileFast(const std::string& file_name);
};

