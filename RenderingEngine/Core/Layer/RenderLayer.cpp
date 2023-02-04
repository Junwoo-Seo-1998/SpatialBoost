/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "RenderLayer.h"

#include "imgui.h"
#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Component/MeshComponent.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Data/TextureData.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Scene/SceneManager.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/FrameBuffer.h"
#include "Core/Data/Texture.h"
void RenderLayer::OnAttach()
{
	vao = VertexArray::CreateVertexArray();
	FrameBufferSpecification spec = { static_cast<unsigned int>(width), static_cast<unsigned int>(height),
{
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::DepthStencil } };
	deferred_fb = FrameBuffer::CreateFrameBuffer(spec);

	constexpr float textureQuad[] = {
		// positions        // texture coords
		 1.f,  1.f, 0.0f, 1.0f, 1.0f,   // top right
		-1.f, -1.f, 0.0f, 0.0f, 0.0f,   // bottom left
		1.f, -1.f, 0.0f, 1.0f, 0.0f,   // bottom right
		-1.f,  1.f, 0.0f, 0.0f, 1.0f,    // top left 
		-1.f, -1.f, 0.0f, 0.0f, 0.0f,   // bottom left
		 1.f,  1.f, 0.0f, 1.0f, 1.0f,   // top right
	};
	quad_buffer = VertexBuffer::CreateVertexBuffer(sizeof(textureQuad));
	quad_buffer->SetData(sizeof(textureQuad), textureQuad);
	quad_buffer->SetDataTypes({
		{0, DataType::Float3},//location=0, pos
		{1, DataType::Float2},//location=1, uv
		});

}

void RenderLayer::OnDetach()
{
	
}

void RenderLayer::OnUpdate()
{
	auto [w, h] = Application::Get().GetWindowSize();
	float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
	
	if(width!=w || height!=h)
	{
		deferred_fb->Resize(w, h);
		width = w;
		height = h;
	}

	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	Entity mainCam = scene->GetMainCamera();
	auto& camComp = mainCam.GetComponent<CameraComponent>();
	camComp.aspect_ratio = AspectRatio;
}

void RenderLayer::OnPreRender()//deferred render
{
	DeferredRender();
}

void RenderLayer::OnRender()//
{
	DeferredRenderQuad();

	ForwardRender();
}



void RenderLayer::OnPostRender()
{
	
}

void RenderLayer::OnGuiRender()
{
	ImGui::Begin("framebuffers");
	ImGui::Checkbox("Copy Depth Info", &copyDepthInfo);
	float AspectRatio = static_cast<float>(width) / static_cast<float>(height);

	ImGui::Text("Position & Normal");
	ImVec2 size{ AspectRatio*200,200 };
	unsigned textureID = deferred_fb->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = deferred_fb->GetColorTexture(1)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::Text("Diffuse & Specular");
	textureID = deferred_fb->GetColorTexture(2)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = deferred_fb->GetColorTexture(3)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });


	ImGui::Text("Emissive & Depth");
	textureID = deferred_fb->GetColorTexture(4)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = deferred_fb->GetDepthTexture()->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
}

void RenderLayer::OnEvent(Event& event)
{

}

void RenderLayer::DeferredRender()
{
	deferred_fb->Bind();
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	std::shared_ptr<Shader> shader = AssetManager::GetShader(AssetManager::GetUUID("Deferred_shader"));
	vao->Bind();
	shader->Use();
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();


	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());


	auto Renderables = registry.group<RendererComponent, TransformComponent, MeshComponent>(entt::get<MaterialComponent>);
	for (auto& Renderable : Renderables)
	{
		auto& rendererComp = Renderables.get<RendererComponent>(Renderable);
		if (!rendererComp.enabled)
			continue;
		auto& transformComp = Renderables.get<TransformComponent>(Renderable);
		auto& meshComp = Renderables.get<MeshComponent>(Renderable);

		if (registry.any_of<MaterialComponent>(Renderable))
		{
			MaterialComponent& material = Renderables.get<MaterialComponent>(Renderable);
			UUID shader_uuid = material.shader;
			//bind shader
			if (shader_uuid != UUID::null)
				continue;
			if (material.mode != RenderMode::Deferred)
				continue;

			shader->TrySetMat4("Matrix.View", world_to_cam);
			shader->TrySetMat4("Matrix.Projection", camComp.GetPerspective());

			glm::mat4 model = transformComp.GetTransform();
			shader->SetMat4("Matrix.Model", model);
			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
			shader->TrySetMat4("Matrix.Normal", normal_matrix);

			//set material
			shader->TrySetMaterial("MatTexture", material.material);

			auto mesh = AssetManager::GetVertexNormalMesh(meshComp.uuid);

			mesh->GetBuffer()->BindToVertexArray();
			mesh->GetUV()->GetPointUVBuffer(UVTypes::Spherical)->BindToVertexArray();
			mesh->GetIndexBuffer()->Bind();

			glDrawElements(GL_TRIANGLES, mesh->GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
		}
		else//use default material
		{

		}
	}
	deferred_fb->UnBind();
}

void RenderLayer::DeferredRenderQuad()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();

	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());

	std::shared_ptr<Shader> shader = AssetManager::GetShader(AssetManager::GetUUID("Deferred_Phong"));
	vao->Bind();
	quad_buffer->BindToVertexArray();
	shader->Use();
	//deferred part
	shader->SetTexture("gPosition", deferred_fb->GetColorTexture(0), 0);
	shader->SetTexture("gNormal", deferred_fb->GetColorTexture(1), 1);
	shader->SetTexture("gDiffuse", deferred_fb->GetColorTexture(2), 2);
	shader->SetTexture("gSpecular", deferred_fb->GetColorTexture(3), 3);
	shader->SetTexture("gEmissive", deferred_fb->GetColorTexture(4), 4);

	{
		auto Lights = registry.view<TransformComponent, LightComponent, RendererComponent>();

		int light_index = 0;
		for (auto& light : Lights)
		{
			auto& renderer = Lights.get<RendererComponent>(light);
			if(!renderer.enabled)
				continue;
			auto& transform = Lights.get<TransformComponent>(light);
			auto& lightComp = Lights.get<LightComponent>(light);
			std::string to_string = std::to_string(light_index++);
			shader->SetInt("Light[" + to_string + "].LightType", static_cast<int>(lightComp.light.m_LightType));
			shader->SetFloat3("Light[" + to_string + "].Position", transform.Position);
			shader->SetFloat3("Light[" + to_string + "].Direction", transform.GetForward());
			shader->SetFloat("Light[" + to_string + "].InnerAngle", lightComp.light.m_Angle.inner);
			shader->SetFloat("Light[" + to_string + "].OuterAngle", lightComp.light.m_Angle.outer);
			shader->SetFloat("Light[" + to_string + "].FallOff", lightComp.light.falloff);

			shader->SetFloat3("Light[" + to_string + "].Ambient", lightComp.light.Ambient);
			shader->SetFloat3("Light[" + to_string + "].Diffuse", lightComp.light.Diffuse);
			shader->SetFloat3("Light[" + to_string + "].Specular", lightComp.light.Specular);
		}
		shader->SetInt("LightNumbers", light_index);
	}
	shader->SetFloat3("CameraPosition", camTransform.Position);

	shader->SetFloat3("globalAmbient", {});

	shader->SetFloat("Attenuation.c1", 1.f);
	shader->SetFloat("Attenuation.c2", 0.4f);
	shader->SetFloat("Attenuation.c3", 0.03f);

	shader->SetFloat3("Fog.Color", { 0.5f,0.5f,0.5f });
	shader->SetFloat("Fog.Near", 0.1f);
	shader->SetFloat("Fog.Far", 195.f);
	//render quad 
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//copy depth
	if (copyDepthInfo)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, deferred_fb->GetFrameBufferID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(
			0, 0, width, height, // source region
			0, 0, width, height, // destination region
			GL_DEPTH_BUFFER_BIT, // field to copy
			GL_NEAREST // filtering mechanism
		);
	}
}

void RenderLayer::ForwardRender()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();
	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::mat4 world_to_cam = Math::BuildCameraMatrixWithDirection(camTransform.Position, camTransform.GetForward(), camTransform.GetUp());
	std::shared_ptr<Shader> shader;

	vao->Bind();

	//render
	{
		auto Renderables = registry.group<RendererComponent, TransformComponent, MeshComponent>(entt::get<MaterialComponent>);
		for (auto& Renderable : Renderables)
		{
			auto& rendererComp = Renderables.get<RendererComponent>(Renderable);
			if (!rendererComp.enabled)
				continue;
			auto& transformComp = Renderables.get<TransformComponent>(Renderable);
			auto& meshComp = Renderables.get<MeshComponent>(Renderable);

			//default
			shader = AssetManager::GetShader("Phong_Shading");
			shader->Use();

			if (registry.any_of<MaterialComponent>(Renderable))
			{
				MaterialComponent& material = Renderables.get<MaterialComponent>(Renderable);
				if (material.mode != RenderMode::Forward)
					continue;
				UUID shader_uuid = material.shader;
				//bind shader
				if (shader_uuid != UUID::null)
				{
					shader = AssetManager::GetShader(shader_uuid);
					shader->Use();
					shader->SetCustomMaterial(material.material.Data);
				}
			}
			else
			{
				continue;
			}
			

			shader->TrySetMat4("Matrix.View", world_to_cam);
			shader->TrySetMat4("Matrix.Projection", camComp.GetPerspective());

			glm::mat4 model = transformComp.GetTransform();
			shader->TrySetMat4("Matrix.Model", model);
			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
			shader->TrySetMat4("Matrix.Normal", normal_matrix);

			auto mesh = AssetManager::GetVertexNormalMesh(meshComp.uuid);

			mesh->GetBuffer()->BindToVertexArray();
			mesh->GetIndexBuffer()->Bind();

			glDrawElements(GL_TRIANGLES, mesh->GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
