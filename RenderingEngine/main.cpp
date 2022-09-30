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
	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 100.f);
	glm::mat4 world_to_cam = Math::BuildCameraMatrix({ 0,2,5 }, { 0,0,0 }, { 0,1,0 });
	bool cullBackFace = true;
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);
	Entity demo_mesh;
	std::string current_mesh = "bunny";
	virtual void Awake()
	{
		AssetManager::LoadMeshFromFile("Assets/4Sphere.obj", "4Sphere");
		AssetManager::LoadMeshFromFile("Assets/bunny_high_poly.obj", "bunny");
		AssetManager::LoadMeshFromFile("Assets/cube2.obj", "cube");
		AssetManager::LoadMeshFromFile("Assets/sphere.obj", "sphere");
		AssetManager::LoadMeshFromFile("Assets/sphere_modified.obj", "sphere_modified");
		AssetManager::LoadMeshFromFile("Assets/triangle.obj", "triangle");
		//generated sphere
		AssetManager::GenerateSphere("Sphere", 0.1f);
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
		

		auto orbit = CreateEntity();
		orbit.GetComponent<TransformComponent>().Scale = { 1,1, 1 };
		orbit.AddComponent<LineRendererComponent>(MeshGenerator::GenerateOrbit(2.f));

	};
	float ie = 0;
	virtual void Update() 
	{
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


		vertex_array->Bind();
		
		demo_mesh.GetComponent<TransformComponent>().Rotation = { 0,ie += 0.01f,0 };

		

		line_shader->Use();
		line_shader->SetMat4("view", world_to_cam);
		line_shader->SetMat4("projection", perspective);
		auto LineMeshes = GetRegistry().view<TransformComponent, LineRendererComponent>();
		for (auto& entity : LineMeshes)
		{
			auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, LineRendererComponent>(entity);
			line_shader->SetMat4("model", TransformComp.GetTransform());

			vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

			glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, LineRendererComp.mesh->GetVertices()->size());
		}

		if (drawNormal)
		{
			if (radio == static_cast<int>(select::DrawFaceNormal))
			{
				auto LineMeshes = GetRegistry().view<TransformComponent, FaceNormalLineRendererComponent>();
				for (auto& entity : LineMeshes)
				{
					auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, FaceNormalLineRendererComponent>(entity);
					line_shader->SetMat4("model", TransformComp.GetTransform());

					vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

					glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, LineRendererComp.mesh->GetVertices()->size());
				}
			}
			else
			{
				auto LineMeshes = GetRegistry().view<TransformComponent, VertexNormalLineRendererComponent>();
				for (auto& entity : LineMeshes)
				{
					auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, VertexNormalLineRendererComponent>(entity);
					line_shader->SetMat4("model", TransformComp.GetTransform());

					vertex_array->AttachBuffer(*LineRendererComp.mesh->GetBuffer());

					glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, LineRendererComp.mesh->GetVertices()->size());
				}
			}
		}

		light_shader->Use();
		light_shader->SetMat4("view", world_to_cam);
		light_shader->SetMat4("projection", perspective);
		light_shader->SetFloat3("LightPos", { 0,0,10 });
		if (radio == static_cast<int>(select::DrawFaceNormal))
		{
			auto Meshes = GetRegistry().view<TransformComponent, FaceNormalMeshRendererComponent>();
			for (auto& entity : Meshes)
			{
				auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, FaceNormalMeshRendererComponent>(entity);
				light_shader->SetMat4("model", TransformComp.GetTransform());

				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());

				if (MeshRendererComp.mesh->GetUseIndex())
				{
					vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
					glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), MeshRendererComp.mesh->GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
				}
				else
				{
					glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, MeshRendererComp.mesh->GetVertices()->size());
				}

			}
		}
		else
		{
			auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent>();
			for (auto& entity : Meshes)
			{
				auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent>(entity);
				light_shader->SetMat4("model", TransformComp.GetTransform());

				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());

				if (MeshRendererComp.mesh->GetUseIndex())
				{
					vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
					glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), MeshRendererComp.mesh->GetIndices()->size(), GL_UNSIGNED_INT, nullptr);
				}
				else
				{
					glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, MeshRendererComp.mesh->GetVertices()->size());
				}

			}
		}
		
	}
	virtual void LateUpdate()
	{
		ImGui::Begin("Control");
		const char* items[] = 
		{
			"4Sphere", "bunny", "cube", "sphere", "sphere_modified", "triangle"
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
		ImGui::Checkbox("CullBackFace", &cullBackFace);
		ImGui::Checkbox("DrawNormal", &drawNormal);
		ImGui::RadioButton("DrawVertexNormal", &radio, static_cast<int>(select::DrawVertexNormal)); ImGui::SameLine();
		ImGui::RadioButton("DrawFaceNormal", &radio, static_cast<int>(select::DrawFaceNormal));
		ImGui::End();
	}
	virtual void OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& event)->bool
		{
			auto [width, height] = event.GetWidthAndHeight();
			float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
			perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), AspectRatio, 0.1f, 100.f);
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