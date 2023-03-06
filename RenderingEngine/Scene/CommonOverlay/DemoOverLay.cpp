/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#include "DemoOverLay.h"

#include <entt/entt.hpp>

#include "imgui.h"
#include "Core/Application.h"
#include "Core/Component/DemoComponent.h"
#include "Core/Scene/SceneManager.h"
void DemoOverLay::OnAttach()
{
	current_mesh = "g0";
}

void DemoOverLay::OnDetach()
{

}

void DemoOverLay::OnUpdate()
{
	
}

void DemoOverLay::OnPreRender()
{

}

void DemoOverLay::OnRender()
{

}

void DemoOverLay::OnPostRender()
{

}

void DemoOverLay::OnGuiRender()
{
	auto scene = Application::Get().GetSceneManager()->GetCurrentScene();
	entt::registry& registry = scene->GetRegistry();

	ImGui::Begin("Control");
	{
		/*static const char* items[] =
		{
			"4Sphere", "bunny", "cube", "sphere", "sphere_modified","GeneratedSphere"
		};
		ImGui::Text("Mesh");
		if (ImGui::BeginCombo("##combo", current_mesh.c_str()))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				bool is_selected = (current_mesh == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
					current_mesh = items[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		auto view = registry.view<DemoComponent>();
		for (auto entt : view)
		{
			auto& Comp = view.get<DemoComponent>(entt);
			Comp.meshName = current_mesh;
		}*/
	}
	{
		/*auto view = registry.view<DemoControlComponent>();
		for (auto entt:view)
		{
			auto& Comp = view.get<DemoControlComponent>(entt);
			ImGui::Checkbox("Stop Rotation", &Comp.StopRotation);
			ImGui::DragInt("Light Numbers", &Comp.LightNumber, 1, 1, 16);
		}*/
	}
	ImGui::End();
}

void DemoOverLay::OnEvent(Event& event)
{

}
