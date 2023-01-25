#include "RenderLayer.h"

#include "Core/AssetManager.h"
#include "Core/Graphics/VertexArray.h"

void RenderLayer::OnAttach()
{
	vao = VertexArray::CreateVertexArray();
}

void RenderLayer::OnDetach()
{

}

void RenderLayer::OnUpdate()
{

}

void RenderLayer::OnRender()
{
	auto shader = AssetManager::GetShader("Phong_Shading");
}

void RenderLayer::OnPostRender()
{
	
}

void RenderLayer::OnGuiRender()
{

}

void RenderLayer::OnEvent(Event& event)
{

}
