#pragma once
#include <string>
#include"Core/Data/Mesh.h"
class ObjParser
{
public:
	static Mesh LoadFile(const std::string& file_name);
	static Mesh LoadFaceNormalLineMesh(const std::string& file_name, float line_length = 0.1f);
	static Mesh LoadVertexNormalLineMesh(const std::string& file_name, float line_length = 0.1f);
private:
	static void LoadFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<Face>& loaded_faces);
	static void ParseFile(const std::string& file_name, std::vector<glm::vec3>& loaded_points, std::vector<unsigned int>& loaded_indices);

};

