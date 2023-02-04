#include "DemoOverLay.h"

#include <entt/entt.hpp>

#include "imgui.h"
#include "Core/Application.h"
#include "Core/Component/DemoComponent.h"
#include "Core/Scene/SceneManager.h"
void DemoOverLay::OnAttach()
{
	current_mesh = "bunny";
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
		static const char* items[] =
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
		}
	}
	{
		auto view = registry.view<DemoControlComponent>();
		for (auto entt:view)
		{
			auto& Comp = view.get<DemoControlComponent>(entt);
			ImGui::Checkbox("Stop Rotation", &Comp.StopRotation);
			ImGui::DragInt("Light Numbers", &Comp.LightNumber, 1, 1, 16);
		}
	}
	ImGui::End();
}

void DemoOverLay::OnEvent(Event& event)
{

}
