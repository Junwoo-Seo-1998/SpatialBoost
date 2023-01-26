#include "RenderLayer.h"

#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/MeshComponent.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Graphics/Shader.h"

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
	vao->Bind();

	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();

	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());

	shader->SetMat4("Matrix.View", world_to_cam);
	shader->SetMat4("Matrix.Projection", camComp.GetPerspective());

	auto Renderables = registry.view<RendererComponent, TransformComponent, MeshComponent>();
	for (auto& Renderable: Renderables)
	{
		auto& rendererComp = Renderables.get<RendererComponent>(Renderable);
		if (!rendererComp.enabled)
			continue;

		auto& transformComp = Renderables.get<TransformComponent>(Renderable);
		auto& meshComp = Renderables.get<MeshComponent>(Renderable);

		glm::mat4 model = transformComp.GetTransform();
		shader->SetMat4("Matrix.Model", model);
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
		shader->SetMat4("Matrix.Normal", normal_matrix);

		auto mesh = AssetManager::GetVertexNormalMesh(meshComp.uuid);

		mesh->GetBuffer()->BindToVertexArray();
		mesh->GetIndexBuffer()->Bind();

		glDrawElements(GL_TRIANGLES, mesh->GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
	}
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
