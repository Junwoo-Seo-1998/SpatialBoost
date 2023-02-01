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

void RenderLayer::OnAttach()
{
	vao = VertexArray::CreateVertexArray();
	FrameBufferSpecification spec = { 1024, 1024,
{
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::RGBA32F,
		FrameBufferFormat::Depth } };
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
	
}

void RenderLayer::OnPreRender()//deferred render
{
	deferred_fb->Bind();
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			if(material.mode!=RenderMode::Deferred)
				continue;

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
	deferred_fb->UnBind();
	auto [w, h] = Application::Get().GetWindowSize();
	glViewport(0, 0, w, h);
}

void RenderLayer::OnRender()//
{
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

	shader->SetInt("LightNumbers", 1);
	std::string to_string = std::to_string(0);
	shader->SetInt("Light[" + to_string + "].LightType", 1);
	glm::vec3 lightPos = glm::vec3(10.f, 0, 0);
	shader->SetFloat3("Light[" + to_string + "].Position", lightPos);
	shader->SetFloat3("Light[" + to_string + "].Direction", glm::vec3{ 0, 0, 0 } - lightPos);
	shader->SetFloat("Light[" + to_string + "].InnerAngle", glm::radians(45.f));
	shader->SetFloat("Light[" + to_string + "].OuterAngle", glm::radians(50.f));
	shader->SetFloat("Light[" + to_string + "].FallOff", 32.f);

	shader->SetFloat3("Light[" + to_string + "].Ambient", {0.f,0.f,0.f});
	shader->SetFloat3("Light[" + to_string + "].Diffuse", { 1.f,1.f,1.f });
	shader->SetFloat3("Light[" + to_string + "].Specular", { 0.0f,0.0f,0.0f });


	shader->SetFloat3("CameraPosition", camTransform.Position);
	shader->SetFloat3("Material.Ambient", { 0.1f,0.1f,0.1f });
	shader->SetFloat3("Material.Diffuse", { 0.8f,0.8f,0.8f });
	shader->SetFloat3("Material.Specular", { 0.1f,0.1f,0.1f });
	shader->SetFloat("Material.Shininess", 32.f);
	shader->SetFloat3("Material.Emissive", {});

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
	glBindFramebuffer(GL_READ_FRAMEBUFFER, deferred_fb->GetFrameBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	auto [w, h] = Application::Get().GetWindowSize();
	glBlitFramebuffer(
		0, 0, 1024, 1024, // source region
		0, 0, w, h, // destination region
		GL_DEPTH_BUFFER_BIT, // field to copy
		GL_NEAREST // filtering mechanism
	);


	auto Renderables = registry.group<RendererComponent, TransformComponent, MeshComponent>(entt::get<MaterialComponent>);
	for (auto& Renderable: Renderables)
	{
		auto& rendererComp = Renderables.get<RendererComponent>(Renderable);
		if (!rendererComp.enabled)
			continue;
		auto& transformComp = Renderables.get<TransformComponent>(Renderable);
		auto& meshComp = Renderables.get<MeshComponent>(Renderable);

		//default
		shader = AssetManager::GetShader("Phong_Shading");

		if(registry.any_of<MaterialComponent>(Renderable))
		{
			MaterialComponent& material = Renderables.get<MaterialComponent>(Renderable);
			if(material.mode!=RenderMode::Forward)
				continue;
			UUID shader_uuid = material.shader;
			//bind shader
			if (shader_uuid != UUID::null)
			{
				shader = AssetManager::GetShader(shader_uuid);
				shader->Use();
				for(auto& pair:material.material.data)
				{
					switch(pair.second.type)
					{
						case DataType::Bool:
							shader->SetInt(pair.first, pair.second.data.Bool);
							break;
						case DataType::Int:
							shader->SetInt(pair.first, pair.second.data.Int);
							break;
						case DataType::Float:
							shader->SetFloat(pair.first, pair.second.data.Float);
							break;
						case DataType::Float3:
							shader->SetFloat3(pair.first, pair.second.data.Float3);
							break;
						case DataType::Float4:
							shader->SetFloat4(pair.first, pair.second.data.Float4);
							break;
						case DataType::Mat4:
							shader->SetMat4(pair.first, pair.second.data.Mat4);
							break;
					default: 
						break;
					}
				}
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



void RenderLayer::OnPostRender()
{
	
}

void RenderLayer::OnGuiRender()
{
	ImGui::Begin("framebuffers");
	constexpr ImVec2 size{ 200,200 };
	unsigned textureID = deferred_fb->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = deferred_fb->GetColorTexture(1)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	textureID = deferred_fb->GetDepthTexture()->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
}

void RenderLayer::OnEvent(Event& event)
{

}
