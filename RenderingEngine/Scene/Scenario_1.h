#pragma once
#include <glm/glm.hpp>

#include "Core/Scene/Scene.h"
#include "Core/Utils/Math.h"

class VertexBuffer;
class Shader;
class VertexArray;
enum class select
{
	DrawVertexNormal = 0,
	DrawFaceNormal
};
class Scenario_1 : public Scene
{
public:
	Scenario_1(Application& app);
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
	std::shared_ptr<Shader> light_shader;
	std::shared_ptr<Shader> line_shader;
	std::shared_ptr<VertexBuffer> buffer;
	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 1000.f);
	glm::mat4 world_to_cam = Math::BuildCameraMatrix({ 0,2,8 }, { 0,0,0 }, { 0,1,0 });
	glm::vec3 light_pos = { 0,0,2 };
	bool cullBackFace = true;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);
	Entity demo_mesh;
	Entity orbit;
	std::string current_mesh = "bunny";
	glm::vec4 color = { 0.5f,0.5f,0.5f,1.f };
	glm::vec4 line_color = { 1.f,1.f, 0.f,1.f };
	float m_LastTime = 0.f;
	float m_center_speed = 1.f;
	float m_orbit_speed = 0.5f;
};
