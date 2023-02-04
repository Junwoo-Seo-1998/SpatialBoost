#include "LineRenderLayer.h"

#include "imgui.h"
#include "Core/AssetManager.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Graphics/Shader.h"
#include "Core/Scene/Entity.h"
#include "Core/Utils/Math.h"
#include "Core/Application.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneManager.h"
#include "Scene/DemoEnums/DemoEnums.h"

void LineRenderLayer::OnAttach()
{
	vao = VertexArray::CreateVertexArray();
}

void LineRenderLayer::OnDetach()
{

}

void LineRenderLayer::OnUpdate()
{

}

void LineRenderLayer::OnPreRender()
{

}

void LineRenderLayer::OnRender()
{

}

void LineRenderLayer::OnPostRender()
{
	vao->Bind();
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());
	std::shared_ptr<Shader> shader;
	//line render
	{
		shader = AssetManager::GetShader("line_shader");
		shader->Use();
		//default
		shader->SetFloat4("BaseColor", lineColor);
		shader->SetMat4("Matrix.View", world_to_cam);
		shader->SetMat4("Matrix.Projection", camComp.GetPerspective());

		auto LineMeshes = registry.view<TransformComponent, LineRendererComponent>();
		for (auto& entity : LineMeshes)
		{
			auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, LineRendererComponent>(entity);
			shader->SetMat4("Matrix.Model", TransformComp.GetTransform());

			LineRendererComp.mesh->GetBuffer()->BindToVertexArray();

			glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
		}

	}

	
	if (drawNormal)
	{
		shader = AssetManager::GetShader("line_shader");
		shader->Use();

		//default
		shader->SetFloat4("BaseColor", lineColor);
		shader->SetMat4("Matrix.View", world_to_cam);
		shader->SetMat4("Matrix.Projection", camComp.GetPerspective());

		if (radio == static_cast<int>(select::DrawFaceNormal))
		{
			auto FaceNormalLineMeshes = registry.view<TransformComponent, FaceNormalLineRendererComponent>();
			for (auto& entity : FaceNormalLineMeshes)
			{
				auto [TransformComp, LineRendererComp] = FaceNormalLineMeshes.get<TransformComponent, FaceNormalLineRendererComponent>(entity);
				shader->SetMat4("Matrix.Model", TransformComp.GetTransform());

				LineRendererComp.mesh->GetBuffer()->BindToVertexArray();

				glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
			}
		}
		else
		{
			auto VertexNormalLineMeshes = registry.view<TransformComponent, VertexNormalLineRendererComponent>();
			for (auto& entity : VertexNormalLineMeshes)
			{
				auto [TransformComp, LineRendererComp] = VertexNormalLineMeshes.get<TransformComponent, VertexNormalLineRendererComponent>(entity);
				shader->SetMat4("Matrix.Model", TransformComp.GetTransform());

				LineRendererComp.mesh->GetBuffer()->BindToVertexArray();

				glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
			}
		}
	}

}

void LineRenderLayer::OnGuiRender()
{
	ImGui::Begin("Debug Drawing");
	ImGui::Checkbox("DrawNormal", &drawNormal);
	ImGui::RadioButton("VertexNormals", &radio, static_cast<int>(select::DrawVertexNormal)); ImGui::SameLine();
	ImGui::RadioButton("FaceNormals", &radio, static_cast<int>(select::DrawFaceNormal));
	ImGui::ColorEdit3("Line Color", &lineColor.x);
	ImGui::End();
}

void LineRenderLayer::OnEvent(Event& event)
{

}
