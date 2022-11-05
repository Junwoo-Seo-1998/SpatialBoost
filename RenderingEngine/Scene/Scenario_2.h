#pragma once
#include <glm/glm.hpp>

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
	void Update() override;
	void LateUpdate() override;
	void OnDisable() override;
	void OnDestroy() override;
	void OnEvent(Event& event) override;
private:
	std::shared_ptr<VertexArray> vertex_array;
	std::string selected_shader;
	std::shared_ptr<Shader> current_shader;
	std::shared_ptr<Shader> line_shader;
	std::shared_ptr<VertexBuffer> buffer;
	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 1000.f);
	glm::mat4 world_to_cam = Math::BuildCameraMatrix({ 0,2,8 }, { 0,0,0 }, { 0,1,0 });
	glm::vec3 light_pos = { 0,0,2 };

	bool StopRotation = false;

	bool cullBackFace = true;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);

	Entity demo_mesh;
	Entity orbit;
	Entity Lights[16];

	std::string current_mesh = "bunny";


	glm::vec4 Mat_Ambient = { 0.001f,0.001f,0.001f,1.f };
	glm::vec4 Mat_Emissive{ 0.0f };

	int light_number = 8;
	int light_type = 0;
	int current_light = 1;

	glm::vec4 line_color = { 1.f,1.f, 0.f,1.f };
	glm::vec4 fog_color{ 0.5f };
	float fog_near = 0.1f;
	float fog_far = 20.f;


	float m_LastTime = 0.f;
	float m_center_speed = 1.f;
	float m_orbit_speed = 0.5f;

	bool useCpu = false;
	int TextureEntity = 0;
	int current_uv_method = 2;
	std::shared_ptr<Layer> overlay;
};
