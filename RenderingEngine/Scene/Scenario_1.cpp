/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_2
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Scenario_1.h"

#include <iostream>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/quaternion.hpp>

#include "imgui.h"
#include "CommonOverlay/CommonOverlay.h"
#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/Time.h"
#include "Core/Component/CameraComponent.h"
#include "Core/Component/DemoComponent.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Component/MeshComponent.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Event/ApplicationEvents/ApplicationEvents.h"
#include "Core/Utils/MeshGenerator.h"
#include "GLFW/glfw3.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Input/Input.h"
#include "Core/Layer/LayerManager.h"
#include "Core/Layer/RenderLayer.h"
#include "Core/Layer/SkyboxRenderLayer.h"

Scenario_1::Scenario_1(Application& app)
	: Scene(app)
{
}

void Scenario_1::Awake()
{
}

void Scenario_1::OnEnable()
{
}

void Scenario_1::Start()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	MainCamera.GetComponent<TransformComponent>().Position = { 0,5,10 };
	//MainCamera.GetComponent<TransformComponent>().Rotation = { glm::radians(-15.f),0, 0 };

	demo_ctrl = CreateEntity();
	demo_ctrl.AddComponent<DemoControlComponent>();

	demo_mesh = CreateEntity();
	demo_mesh.AddComponent<DemoComponent>();
	demo_mesh.GetComponent<TransformComponent>().Scale = { 1,1,1 };
	demo_mesh.AddComponent<RendererComponent>();
	demo_mesh.AddComponent<MaterialComponent>();
	demo_mesh.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh(AssetManager::GetUUID("bunny")));
	demo_mesh.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh(AssetManager::GetUUID("bunny")));
	demo_mesh.AddComponent<MeshComponent>("bunny");


	Entity plane = CreateEntity();
	plane.GetComponent<TransformComponent>().Position = { 0,-1,0 };
	plane.GetComponent<TransformComponent>().Scale = { 5.f,5.f,1.f };
	plane.GetComponent<TransformComponent>().Rotation = { glm::radians(-90.f),0.f,0.f };
	plane.AddComponent<MaterialComponent>();
	plane.AddComponent<RendererComponent>();
	plane.AddComponent<MeshComponent>("Plane");

	
	float radius = 3.f;
	orbit = CreateEntity();
	auto& parent_transform = orbit.GetComponent<TransformComponent>();
	orbit.AddComponent<LineRendererComponent>(MeshGenerator::GenerateOrbit(radius));

	float d_theta = 2.f * glm::pi<float>() / static_cast<float>(16);
	float theta = 0.f;
	for (int step = 0; step < 16; ++step)
	{
		glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
		auto GeneratedSphere = CreateEntity();
		auto& transform = GeneratedSphere.GetComponent<TransformComponent>();
		transform.Position = position;
		transform.LookAtDir(glm::vec3{ 0,0,0 } - position);
		GeneratedSphere.AddComponent<RendererComponent>();
		GeneratedSphere.AddComponent<MeshComponent>("GeneratedOrbitSphere");
		GeneratedSphere.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh(AssetManager::GetUUID("GeneratedOrbitSphere")));
		GeneratedSphere.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh(AssetManager::GetUUID("GeneratedOrbitSphere")));
		auto& mat=GeneratedSphere.AddComponent<MaterialComponent>("light_shader");
		mat.mode = RenderMode::Forward;
		mat["BaseColor"] = glm::vec4{ 1,1,1,1 };
		auto& Light = GeneratedSphere.AddComponent<LightComponent>();
		Light.light.m_LightType = LightType::PointLight;
		theta += d_theta;
	}

}


void Scenario_1::Update()
{
	float dt = Time::GetDelta();
	auto& ctrl = demo_ctrl.GetComponent<DemoControlComponent>();

	glm::vec3 orbit_rot;
	if (!ctrl.StopRotation)
	{
		auto& orbit_trans = orbit.GetComponent<TransformComponent>();
		orbit_trans.Rotation.y += orbit_trans.Rotation.y >= 360.f ? -360.f + 0.5f * dt : 0.5f * dt;
		orbit_rot = orbit_trans.Rotation;
	}

	auto rot_mat = glm::toMat4(glm::quat(orbit_rot));
	{//light update
		float radius = 3.f;
		float d_theta = 2.f * glm::pi<float>() / static_cast<float>(ctrl.LightNumber);
		float theta = 0.f;
		auto Lights = GetRegistry().view<TransformComponent, LightComponent, RendererComponent>();
		int i = 0;
		for (auto& entity : Lights)
		{
			auto [TransformComp, Light, Renderer] = Lights.get<TransformComponent, LightComponent, RendererComponent>(entity);

			if (i <= ctrl.LightNumber)
				Renderer.enabled = true;
			else
				Renderer.enabled = false;

			glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
			TransformComp.Position = rot_mat * glm::vec4{ position ,1.f };
			TransformComp.LookAtDir(glm::vec3{ 0,0,0 } - position);
			theta += d_theta;
			i++;
		}
	}
}

void Scenario_1::PostUpdate()
{

}

void Scenario_1::OnDisable()
{
}

void Scenario_1::OnDestroy()
{
}

void Scenario_1::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);

	dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& event)->bool
		{
			auto [width, height] = event.GetWidthAndHeight();
			float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
			auto& Cam = MainCamera.GetComponent<CameraComponent>();
			Cam.aspect_ratio = AspectRatio;
			return true;
		});
}
