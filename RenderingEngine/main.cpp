/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: entry point for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include"Core/Application.h"
#include"Core/EntryPoint.h"
#include"Core/Scene/Scene.h"
#include<memory>

#include "Core/AssetManager.h"
#include "Core/Layer/CameraLayer.h"
#include "Core/Layer/LayerManager.h"
#include "Core/Layer/LineRenderLayer.h"
#include "Core/Layer/RenderLayer.h"
#include "Core/Layer/SkyboxRenderLayer.h"
#include "Core/Utils/File.h"
#include "Scene/Scenario_1.h"
#include "Scene/CommonOverlay/DemoOverLay.h"


class MyApp : public Application
{
	virtual void UserDefinedInit() override
	{
		AssetManager::LoadShaderFromFile("skybox_shader", "Assets/Shaders/common.glsl", "Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag");
		AssetManager::LoadShaderFromFile("line_shader", "Assets/Shaders/common.glsl", "Assets/Shaders/normal.vert", "Assets/Shaders/normal.frag");
		AssetManager::LoadShaderFromFile("light_shader", "Assets/Shaders/common.glsl", "Assets/Shaders/light.vert", "Assets/Shaders/light.frag");
		AssetManager::LoadShaderFromFile("Phong_Shading", "Assets/Shaders/common.glsl", "Assets/Shaders/phong_shading.vert", "Assets/Shaders/phong_shading.frag");
		AssetManager::LoadShaderFromFile("Deferred_shader", "Assets/Shaders/common.glsl", "Assets/Shaders/deferred.vert", "Assets/Shaders/deferred.frag");
		AssetManager::LoadShaderFromFile("Deferred_Phong", "Assets/Shaders/common.glsl", "Assets/Shaders/deferred_phong.vert", "Assets/Shaders/deferred_phong.frag");

		AssetManager::LoadMeshFromFile("Assets/4Sphere.obj", "4Sphere");
		AssetManager::LoadMeshFromFile("Assets/bunny_high_poly.obj", "bunny");
		AssetManager::LoadMeshFromFile("Assets/cube2.obj", "cube");
		AssetManager::LoadMeshFromFile("Assets/sphere.obj", "sphere");
		AssetManager::LoadMeshFromFile("Assets/sphere_modified.obj", "sphere_modified");

		AssetManager::LoadTextureFromFile("diff", "Assets/metal_roof_diff_512x512.png");
		AssetManager::LoadTextureFromFile("spec", "Assets/metal_roof_spec_512x512.png");
		AssetManager::LoadTextureFromFile("sky_front", "Assets/front.jpg");
		AssetManager::LoadTextureFromFile("sky_back", "Assets/back.jpg");
		AssetManager::LoadTextureFromFile("sky_left", "Assets/left.jpg");
		AssetManager::LoadTextureFromFile("sky_right", "Assets/right.jpg");
		AssetManager::LoadTextureFromFile("sky_bottom", "Assets/bottom.jpg");
		AssetManager::LoadTextureFromFile("sky_top", "Assets/top.jpg");
		//generated
		AssetManager::GeneratePlane("Plane");
		AssetManager::GenerateSphere("GeneratedOrbitSphere", 0.1f, 10, 10);
		AssetManager::GenerateSphere("GeneratedSphere", 1.f);
		AssetManager::GenerateSkybox();

		SetCurrentScene(std::make_shared<Scenario_1>(Application::Get()));


		Application::Get().GetLayerManager()->PushLayer(std::make_shared<RenderLayer>());
		Application::Get().GetLayerManager()->PushLayer(std::make_shared<LineRenderLayer>());
		Application::Get().GetLayerManager()->PushLayer(std::make_shared<SkyboxRenderLayer>());
		Application::Get().GetLayerManager()->PushLayer(std::make_shared<CameraLayer>());
		Application::Get().GetLayerManager()->PushOverlay(std::make_shared<DemoOverLay>());
	}
};
std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<MyApp>();
}