#include "Mesh.h"

std::vector<Vertex>& Mesh::GetVertices()
{
	return m_Vertices;
}

std::vector<Face>& Mesh::GetFaces()
{
	return m_Faces;
}
