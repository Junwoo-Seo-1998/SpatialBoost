#include "RendererComponent.h"

LineRendererComponent::LineRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Mesh> mesh)
	:mesh(mesh)
{
}
