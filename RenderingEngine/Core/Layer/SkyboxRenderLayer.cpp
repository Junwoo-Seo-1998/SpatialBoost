#include "SkyboxRenderLayer.h"

#include "Core/Application.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Graphics/Shader.h"
#include "Core/Scene/SceneManager.h"

#include "entt/entt.hpp"

void SkyboxRenderLayer::OnAttach()
{
	skybox_shader = AssetManager::GetShader("skybox_shader");
	vao = std::make_shared<VertexArray>();
}

void SkyboxRenderLayer::OnDetach()
{
	
}

void SkyboxRenderLayer::OnUpdate()
{
	
}

void SkyboxRenderLayer::OnPostRender()
{
	Entity mainCam = Application::Get().GetSceneManager()->GetCurrentScene()->GetMainCamera();
	auto& transform = mainCam.GetComponent<TransformComponent>();

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	vao->Bind();
	skybox_shader->Use();
	vao->AttachBuffer(*AssetManager::GetSkybox());
	auto [width, height] = Application::Get().GetWindowSize();
	const float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
	glm::mat3 world_to_cam = Math::BuildCameraMatrixWithDirection(transform.Position, transform.GetForward(), transform.GetUp());

	skybox_shader->SetMat4("view", glm::mat3{ world_to_cam });
	skybox_shader->SetMat4("projection", Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), AspectRatio, 0.1f, 1000.f));
	skybox_shader->SetTexture("skybox[0]", AssetManager::GetTexture("sky_left"), 0);
	skybox_shader->SetTexture("skybox[1]", AssetManager::GetTexture("sky_right"), 1);

	skybox_shader->SetTexture("skybox[2]", AssetManager::GetTexture("sky_front"), 2);
	skybox_shader->SetTexture("skybox[3]", AssetManager::GetTexture("sky_back"), 3);

	skybox_shader->SetTexture("skybox[4]", AssetManager::GetTexture("sky_bottom"), 4);
	skybox_shader->SetTexture("skybox[5]", AssetManager::GetTexture("sky_top"), 5);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyboxRenderLayer::OnGuiRender()
{

}

void SkyboxRenderLayer::OnEvent(Event& event)
{

}
