#include "CommonOverlay.h"

#include "imgui.h"
#include "Core/AssetManager.h"
#include "Core/Event/Event.h"
#include "Core/Application.h"
#include "Scene/Scenario_1.h"
#include "Scene/Scenario_2.h"
#include "Scene/Scenario_3.h"

void CommonOverlay::OnAttach()
{

}

void CommonOverlay::OnDetach()
{

}

void CommonOverlay::OnUpdate()
{
	ImGui::Begin("Scene Control");
	if(ImGui::Button("Scene1"))
	{
		Application::Get().SetCurrentScene(std::make_shared<Scenario_1>(Application::Get()));
	}
	if (ImGui::Button("Scene2"))
	{
		Application::Get().SetCurrentScene(std::make_shared<Scenario_2>(Application::Get()));
	}
	if (ImGui::Button("Scene3"))
	{
		Application::Get().SetCurrentScene(std::make_shared<Scenario_3>(Application::Get()));
	}
	ImGui::End();
}

void CommonOverlay::OnGuiRender()
{

}

void CommonOverlay::OnEvent(Event& event)
{

}
