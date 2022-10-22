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
#include"glad.h"
#include"Core/Utils/Math.h"
#include"Core/Graphics/Shader.h"
#include<memory>
#include <glm/ext/scalar_constants.hpp>
#include "Core/AssetManager.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include"Core/Graphics/Buffer.h"
#include"Core/Graphics/VertexArray.h"
#include "imgui.h"
#include "Core/Event/ApplicationEvents/ApplicationEvents.h"
#include "Core/Utils/MeshGenerator.h"

enum class select
{
	DrawVertexNormal=0,
	DrawFaceNormal
};

class MyScene : public Scene
{
public:
	MyScene(Application& app):Scene(app){}
private:
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<Shader> light_shader;
	std::shared_ptr<Shader> line_shader;
	std::shared_ptr<VertexBuffer> buffer;
	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 1000.f);
	glm::mat4 world_to_cam = Math::BuildCameraMatrix({ 0,2,5 }, { 0,0,0 }, { 0,1,0 });
	glm::vec3 light_pos = { 0,0,10 };
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
	
	
	virtual void Awake()
	{
		AssetManager::LoadMeshFromFile("Assets/4Sphere.obj", "4Sphere");
		AssetManager::LoadMeshFromFile("Assets/bunny_high_poly.obj", "bunny");
		AssetManager::LoadMeshFromFile("Assets/cube2.obj", "cube");
		AssetManager::LoadMeshFromFile("Assets/sphere.obj", "sphere");
		AssetManager::LoadMeshFromFile("Assets/sphere_modified.obj", "sphere_modified");
		//generated sphere
		AssetManager::GenerateSphere("GeneratedOrbitSphere", 0.1f,10,10);
		AssetManager::GenerateSphere("GeneratedSphere", 1.f);
	}

	virtual void OnEnable() {}
	virtual void Start() 
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		line_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/normal.vert", "Assets/Shaders/normal.frag");

		light_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/light.vert", "Assets/Shaders/light.frag");

		vertex_array = std::make_shared<VertexArray>();
		vertex_array->Bind();

		demo_mesh = CreateEntity();
		demo_mesh.GetComponent<TransformComponent>().Scale = { 1,1,1 };
		demo_mesh.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh("bunny"));
		demo_mesh.AddComponent<FaceNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("bunny"));
		demo_mesh.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh("bunny"));
		demo_mesh.AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetVertexNormalMesh("bunny"));
		
		float radius = 2.f;
		orbit = CreateEntity();
		auto& parent_transform = orbit.GetComponent<TransformComponent>();
		orbit.AddComponent<LineRendererComponent>(MeshGenerator::GenerateOrbit(radius));
		float d_theta = 2.f * glm::pi<float>() / static_cast<float>(8);
		float theta = 0.f;
		for (int step = 0; step < 8; ++step)
		{
			glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
			auto GeneratedSphere = CreateEntity();
			GeneratedSphere.GetComponent<TransformComponent>().Position = position;
			GeneratedSphere.GetComponent<TransformComponent>().Parent = &parent_transform;
			GeneratedSphere.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh("GeneratedOrbitSphere"));
			GeneratedSphere.AddComponent<FaceNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("GeneratedOrbitSphere"));
			GeneratedSphere.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh("GeneratedOrbitSphere"));
			GeneratedSphere.AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetVertexNormalMesh("GeneratedOrbitSphere"));
			theta += d_theta;
		}
	}
	virtual void Update() 
	{
		float time = (float)glfwGetTime();
		float dt = time - m_LastTime;
		m_LastTime = time;

		//for gui control
		if(cullBackFace)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
		//for gui control
		demo_mesh.GetComponent<FaceNormalLineRendererComponent>().mesh = AssetManager::GetFaceNormalLineMesh(current_mesh);
		demo_mesh.GetComponent<FaceNormalMeshRendererComponent>().mesh = AssetManager::GetFaceNormalMesh(current_mesh);
		demo_mesh.GetComponent<VertexNormalLineRendererComponent>().mesh = AssetManager::GetVertexNormalLineMesh(current_mesh);
		demo_mesh.GetComponent<VertexNormalMeshRendererComponent>().mesh = AssetManager::GetVertexNormalMesh(current_mesh);

		auto& demo_trans = demo_mesh.GetComponent<TransformComponent>();
		demo_trans.Rotation.y += demo_trans.Rotation.y >= 360.f ? -360.f + m_center_speed * dt : m_center_speed * dt;
		auto& orbit_trans = orbit.GetComponent<TransformComponent>();
		orbit_trans.Rotation.y += orbit_trans.Rotation.y >= 360.f ? -360.f + m_orbit_speed * dt : m_orbit_speed * dt;

		vertex_array->Bind();

		line_shader->Use();
		line_shader->SetMat4("view", world_to_cam);
		line_shader->SetMat4("projection", perspective);
		line_shader->SetFloat4("BaseColor", line_color);
		auto LineMeshes = GetRegistry().view<TransformComponent, LineRendererComponent>();
		for (auto& entity : LineMeshes)
		{
			auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, LineRendererComponent>(entity);
			line_shader->SetMat4("model", TransformComp.GetTransform());

			vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

			glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
		}

		if (drawNormal)
		{
			if (radio == static_cast<int>(select::DrawFaceNormal))
			{
				auto FaceNormalLineMeshes = GetRegistry().view<TransformComponent, FaceNormalLineRendererComponent>();
				for (auto& entity : FaceNormalLineMeshes)
				{
					auto [TransformComp, LineRendererComp] = FaceNormalLineMeshes.get<TransformComponent, FaceNormalLineRendererComponent>(entity);
					line_shader->SetMat4("model", TransformComp.GetTransform());

					vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

					glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
				}
			}
			else
			{
				auto VertexNormalLineMeshes = GetRegistry().view<TransformComponent, VertexNormalLineRendererComponent>();
				for (auto& entity : VertexNormalLineMeshes)
				{
					auto [TransformComp, LineRendererComp] = VertexNormalLineMeshes.get<TransformComponent, VertexNormalLineRendererComponent>(entity);
					line_shader->SetMat4("model", TransformComp.GetTransform());

					vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

					glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(LineRendererComp.mesh->GetVertices()->size()));
				}
			}
		}

		light_shader->Use();
		light_shader->SetMat4("view", world_to_cam);
		light_shader->SetMat4("projection", perspective);
		light_shader->SetFloat3("light.PosOrDir", light_pos);
		light_shader->SetFloat4("BaseColor", color);

		{
			auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent>();
			for (auto& entity : Meshes)
			{
				auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent>(entity);
				glm::mat4 model = TransformComp.GetTransform();
				light_shader->SetMat4("model", model);
				glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
				light_shader->SetMat4("normalMat", normal_matrix);

				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());

				if (MeshRendererComp.mesh->GetUseIndex())
				{
					vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
					glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), static_cast<GLsizei>(MeshRendererComp.mesh->GetIndices()->size()), GL_UNSIGNED_INT, nullptr);
				}
				else
				{
					glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(MeshRendererComp.mesh->GetVertices()->size()));
				}

			}
		}
		
	}
	virtual void LateUpdate()
	{
		ImGui::Begin("Control");
		const char* items[] = 
		{
			"4Sphere", "bunny", "cube", "sphere", "sphere_modified","GeneratedSphere"
		};
		ImGui::Text("Mesh");
		if(ImGui::BeginCombo("##combo",current_mesh.c_str()))
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
		ImGui::Checkbox("CullBackFace", &cullBackFace); ImGui::SameLine();
		ImGui::Checkbox("DrawNormal", &drawNormal);
		ImGui::RadioButton("VertexNormals", &radio, static_cast<int>(select::DrawVertexNormal)); ImGui::SameLine();
		ImGui::RadioButton("FaceNormals", &radio, static_cast<int>(select::DrawFaceNormal));

		ImGui::ColorEdit4("Mesh Color", &color[0]);
		ImGui::ColorEdit4("Line Color", &line_color[0]);
		ImGui::End();
	}
	virtual void OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& event)->bool
		{
			auto [width, height] = event.GetWidthAndHeight();
			float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
			perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), AspectRatio, 0.1f, 1000.f);
			return true;
		});
	}
	virtual void OnDisable() {}
	virtual void OnDestroy() 
	{
		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);
	}
};

class MyApp : public Application
{
	virtual void UserDefinedInit() override
	{
		SetCurrentScene(std::make_shared<MyScene>(*this));
	}
};
std::shared_ptr<Application> CoreMain()
{
	return std::make_shared<MyApp>();
}