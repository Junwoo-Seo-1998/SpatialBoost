#pragma once
#include <memory>
class Mesh;

struct LineRendererComponent
{
	LineRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};

struct MeshRendererComponent
{
	MeshRendererComponent(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> mesh;
};
