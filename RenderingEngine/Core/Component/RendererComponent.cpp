#include "RendererComponent.h"

#include "Core/Data/Mesh.h"

FaceNormalLineRendererComponent::FaceNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh)
	:mesh(mesh)
{
}

VertexNormalLineRendererComponent::VertexNormalLineRendererComponent(std::shared_ptr<LineMesh> mesh)
	:mesh(mesh)
{
}

FaceNormalMeshRendererComponent::FaceNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}

VertexNormalMeshRendererComponent::VertexNormalMeshRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}
