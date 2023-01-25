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
#pragma once
#include <glm/glm.hpp>

#include "Core/Scene/Scene.h"
#include "Core/Utils/Math.h"
#include "DemoEnums/DemoEnums.h"
#include "Core/Data/UV.h"
#include "Core/Data/Light.h"
#include "Core/Graphics/FrameBuffer.h"
class Layer;
class VertexBuffer;
class Shader;
class VertexArray;

class Scenario_1 : public Scene
{
public:
	Scenario_1(Application& app);
	void Awake() override;
	void OnEnable() override;
	void Start() override;
	void Update() override;
	void PostUpdate() override;
	void OnDisable() override;
	void OnDestroy() override;
	void OnEvent(Event& event) override;
private:

	bool StopRotation=false;

	bool cullBackFace = true;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);

	Entity demo_mesh;
	Entity orbit;

	std::string current_mesh = "GeneratedSphere";

	glm::vec3 Global_Ambient_Color = { 0.01f,0.0f,0.01f };

	float c1 = 1.f;
	float c2 = 0.4f;
	float c3 = 0.03f;

	glm::vec4 Mat_Ambient = { 0.001f,0.001f,0.001f,1.f };
	glm::vec4 Mat_Emissive{ 0.0f };

	int light_number = 8;
	int light_type = 0;
	glm::vec4 Light_Ambient_Color = { 0.5f,0.5f,0.5f,1.f };
	glm::vec4 Light_Diffuse_Color = { 0.8f,0.8f,0.8f,1.f };
	glm::vec4 Light_Specular_Color = { 0.5f,0.5f,0.5f,1.f };
	glm::vec4 line_color = { 1.f,1.f, 0.f,1.f };
	glm::vec4 fog_color{ 0.5f };
	float fog_near = 0.1f;
	float fog_far = 195.f;

	float inner = 30.f;
	float outer = 45.f;
	float falloff = 32.f;

	float m_center_speed = 1.f;
	float m_orbit_speed = 0.5f;

	int TextureEntity = 0;
	int current_uv_method = 2;
	std::shared_ptr<Layer> overlay;
};
