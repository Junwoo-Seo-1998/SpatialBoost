#pragma once
#include<glm/glm.hpp>
#include<vector>
#include"Core/Data/Vertex.h"
#include"Core/Data/Face.h"
class Mesh
{
public:
	std::vector<Vertex>& GetVertices();
	std::vector<Face>& GetFaces();
private:
	std::vector<Vertex> m_Vertices;
	std::vector<Face> m_Faces;
};

