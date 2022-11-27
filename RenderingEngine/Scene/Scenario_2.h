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

#include "Core/Graphics/FrameBuffer.h"
#include "Core/Scene/Scene.h"
#include "Core/Utils/Math.h"
#include "DemoEnums/DemoEnums.h"

class Layer;
class VertexBuffer;
class Shader;
class VertexArray;

class Scenario_2 : public Scene
{
public:
	Scenario_2(Application& app);
	void Awake() override;
	void OnEnable() override;
	void Start() override;
	void DrawEnv(const glm::mat4& worldToCam);
	void UpdateFrameBuffers(const glm::vec3& object_pos);
	void Update() override;
	void LateUpdate() override;
	void OnDisable() override;
	void OnDestroy() override;
	void OnEvent(Event& event) override;
private:
	std::shared_ptr<FrameBuffer> FrameBuffers[6];
	float reDrawTime = 0.025f;
	float reDrawAcc = 0.f;
	std::shared_ptr<VertexArray> vertex_array;
	std::string selected_shader;
	std::shared_ptr<Shader> current_shader;
	std::shared_ptr<Shader> line_shader;
	std::shared_ptr<VertexBuffer> buffer;
	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 1000.f);
	glm::mat4 world_to_cam{};
	glm::vec3 campos = { 0,2,8 };

	bool StopRotation = false;

	bool cullBackFace = true;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);

	Entity demo_mesh;
	Entity orbit;
	Entity Lights[16];

	std::string current_mesh = "GeneratedSphere";

	glm::vec3 Global_Ambient_Color = { 0.01f,0.0f,0.01f };

	float c1 = 1.f;
	float c2 = 0.4f;
	float c3 = 0.03f;

	glm::vec4 Mat_Ambient = { 0.001f,0.001f,0.001f,1.f };
	glm::vec4 Mat_Emissive{ 0.0f };
	int MatRadio = static_cast<int>(select::ShowFresnelEffect);
	float Mat_RefractiveIndex = 2.1f;
	float Mat_RGBRefractionRatio = 0.13f;
	float Mat_FresnelPower = 0.5f;

	int light_number = 8;
	int light_type = 0;
	int current_light = 1;

	glm::vec4 line_color = { 1.f,1.f, 0.f,1.f };
	glm::vec4 fog_color{ 0.5f };
	float fog_near = 0.1f;
	float fog_far = 195.f;


	float m_LastTime = 0.f;
	float m_center_speed = 1.f;
	float m_orbit_speed = 0.5f;

	bool useCpu = false;
	int TextureEntity = 0;
	int current_uv_method = 2;
	std::shared_ptr<Layer> overlay;
};
