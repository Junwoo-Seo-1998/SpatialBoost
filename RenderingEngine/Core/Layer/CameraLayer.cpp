#include "CameraLayer.h"

#include "imgui.h"
#include "Core/Application.h"
#include "Core/Time.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Input/Input.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/SceneManager.h"

void CameraLayer::OnAttach()
{

}

void CameraLayer::OnDetach()
{

}

void CameraLayer::OnUpdate()
{
	constexpr float speed = 15.f;
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();

	Entity mainCam = scene->GetMainCamera();
	auto& camTransform = mainCam.GetComponent<TransformComponent>();
	auto& camComp = mainCam.GetComponent<CameraComponent>();

	glm::vec3 movement{ 0,0,0 };

	if (Input::IsPressed(KeyCode::W))
		movement += camTransform.GetForward();
	if (Input::IsPressed(KeyCode::S))
		movement -= camTransform.GetForward();

	if (Input::IsPressed(KeyCode::A))
		movement -= camTransform.GetRight();
	if (Input::IsPressed(KeyCode::D))
		movement += camTransform.GetRight();
	if (Input::IsPressed(KeyCode::Space))
	{
		movement += camTransform.GetUp();
	}
	if (Input::IsPressed(KeyCode::LeftControl))
	{
		movement -= camTransform.GetUp();
	}
	const float len = static_cast<float>(movement.length());
	if (len !=0.f)
	{
		movement /= len;
	}
	camTransform.Position += movement * speed * Time::GetDelta();

	camTransform.LookAtDir(glm::vec3{ 0,0,0 } - camTransform.Position);
	if (glm::length(camTransform.Position) <= 2.f)
		camTransform.Position += camTransform.GetForward() * 8.f;
}

void CameraLayer::OnRender()
{

}

void CameraLayer::OnPostRender()
{

}

void CameraLayer::OnGuiRender()
{
	ImGui::Begin("Control");
	ImGui::TextColored({ 1,0,0,1 }, "WASD to move. Spacebar and Ctrl for Up and Down");
	ImGui::End();
}

void CameraLayer::OnEvent(Event& event)
{

}
