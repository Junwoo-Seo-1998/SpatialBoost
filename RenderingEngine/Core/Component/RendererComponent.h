#pragma once
#include <memory>
class LineMesh;
class Mesh;

struct FaceNormalLineRendererComponent
{
	FaceNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh);

	std::shared_ptr<LineMesh> mesh;
};
struct VertexNormalLineRendererComponent
{
	VertexNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh);

	std::shared_ptr<LineMesh> mesh;
};
struct FaceNormalMeshRendererComponent
{
	FaceNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};
struct VertexNormalMeshRendererComponent
{
	VertexNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};






