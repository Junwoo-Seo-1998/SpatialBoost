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
#include"Core/Utils/ObjParser.h"
#include"glad.h"
#include"Core/Utils/Math.h"
#include"Core/Data/Vertex.h"
#include"Core/Graphics/Shader.h"
#include<memory>

#include "Core/AssetManager.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include"Core/Graphics/Buffer.h"
#include"Core/Graphics/VertexArray.h"
#include "imgui.h"
#include "Core/Event/ApplicationEvents/ApplicationEvents.h"
class MyScene : public Scene
{
public:
	MyScene(Application& app):Scene(app){}
private:
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<Shader> light_shader;
	std::shared_ptr<Shader> line_shader;

	glm::mat4 perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 100.f);

	bool drawNormal = false;

	virtual void Awake() {};
	virtual void OnEnable() {};
	virtual void Start() 
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		line_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/normal.vert", "Assets/Shaders/normal.frag");

		light_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/light.vert", "Assets/Shaders/light.frag");

		vertex_array = std::make_shared<VertexArray>();
		vertex_array->Bind();

		AssetManager::LoadMeshFromFile("Assets/cube2.obj", "bunny");
		auto bunny = CreateEntity();
		bunny.AddComponent<LineRendererComponent>(AssetManager::GetVertexNormalLineMesh("bunny"));
		bunny.AddComponent<MeshRendererComponent>(AssetManager::GetFaceNormalMesh("bunny"));
	};
	float ie = 0;
	virtual void Update() 
	{
		vertex_array->Bind();
		auto world_to_cam = Math::BuildCameraMatrix({ 0,0,10 }, { 0,0,0 }, { 0,1,0 });

		if (drawNormal)
		{
			line_shader->Use();
			line_shader->SetMat4("view", world_to_cam);
			line_shader->SetMat4("projection", perspective);
			auto LineMeshes = GetRegistry().view<TransformComponent, LineRendererComponent>();
			for (auto& entity : LineMeshes)
			{
				auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, LineRendererComponent>(entity);
				line_shader->SetMat4("model", TransformComp.GetTransform());
				for (auto buffer : LineRendererComp.mesh->GetBuffers())
				{
					vertex_array->AttachBuffer(*buffer);
				}

				glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, LineRendererComp.mesh->GetVertices().size());
			}
		}

		light_shader->Use();
		light_shader->SetMat4("view", world_to_cam);
		light_shader->SetMat4("projection", perspective);
		light_shader->SetFloat3("LightPos", { 0,0,10 });
		auto Meshes = GetRegistry().view<TransformComponent, MeshRendererComponent>();
		for (auto& entity : Meshes)
		{
			auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, MeshRendererComponent>(entity);
			light_shader->SetMat4("model", TransformComp.GetTransform());
			for (auto buffer : MeshRendererComp.mesh->GetBuffers())
			{
				vertex_array->AttachBuffer(*buffer);
			}

			if (MeshRendererComp.mesh->GetUseIndex())
			{
				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
				glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), MeshRendererComp.mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, MeshRendererComp.mesh->GetVertices().size());
			}

		}
	}
	virtual void LateUpdate()
	{
		ImGui::Begin("Control");
		ImGui::Checkbox("DrawNormal", &drawNormal);
		//ImGui::RadioButton()
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
	std::cout<<"Vertex:" << sizeof(Vertex) << std::endl;
	return std::make_shared<MyApp>();
}