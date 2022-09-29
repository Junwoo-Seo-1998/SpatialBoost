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
class MyScene : public Scene
{
public:
	MyScene(Application& app):Scene(app){}
private:
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<VertexBuffer> vertex_buffer_class;
	std::shared_ptr<VertexBuffer> vertex_buffer_class_normal;
	std::shared_ptr<ElementBuffer> element_buffer;
	
	std::shared_ptr<Shader> light_shader;
	std::shared_ptr<Shader> line_shader;
	std::shared_ptr<Mesh> shared_mesh;
	Mesh mesh;
	Entity bunny;
	
	virtual void Awake() {};
	virtual void OnEnable() {};
	virtual void Start() 
	{
		//ObjParser parser;
		//mesh = std::move(parser.LoadFile("../Assets/bunny_high_poly.obj"));
		//mesh=parser.LoadFile("../Assets/bunny_high_poly.obj");
		//mesh = parser.LoadFile("../Assets/bunny_high_poly.obj");
		//mesh = parser.LoadFile("../Assets/cube2.obj");
		//mesh = parser.LoadFaceNormalLineMesh("../Assets/cube2.obj", 0.2);
		//mesh = parser.LoadFaceNormalLineMesh("Assets/bunny_high_poly.obj", 0.1);
		//shared_mesh = AssetManager::LoadMeshFromFile("Assets/bunny_high_poly.obj","bunny");
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		

		auto world_to_cam = Math::BuildCameraMatrix({ 0,0,10 }, { 0,0,0 }, { 0,1,0 });
		auto perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 100.f);

		line_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/normal.vert", "Assets/Shaders/normal.frag");
		line_shader->SetMat4("view", world_to_cam);
		line_shader->SetMat4("projection", perspective);

		light_shader = AssetManager::LoadShaderFromFile("Assets/Shaders/light.vert", "Assets/Shaders/light.frag");
		light_shader->SetMat4("view", world_to_cam);
		light_shader->SetMat4("projection", perspective);
		light_shader->SetFloat3("LightPos", { 0,0,10 });

		vertex_array = std::make_shared<VertexArray>();
		vertex_array->Bind();

		/*
		vertex_buffer_class = std::make_shared<VertexBuffer>(mesh.GetVertices().size() * sizeof(glm::vec3));
		vertex_buffer_class->BufferData(mesh.GetVertices().data(), mesh.GetVertices().size() * sizeof(glm::vec3));
		vertex_buffer_class->DescribeData({ {0,Float3}});

		vertex_buffer_class_normal = std::make_shared<VertexBuffer>(mesh.GetNormals().size() * sizeof(glm::vec3));
		vertex_buffer_class_normal->BufferData(mesh.GetNormals().data(), mesh.GetNormals().size() * sizeof(glm::vec3));
		vertex_buffer_class_normal->DescribeData({ {1,Float3} });

		vertex_array->AttachBuffer(*vertex_buffer_class);
		vertex_array->AttachBuffer(*vertex_buffer_class_normal);
		element_buffer = std::make_shared<ElementBuffer>(mesh.GetIndices());
		vertex_array->AttachBuffer(*element_buffer);

		*/
		

		AssetManager::LoadMeshFromFile("Assets/cube2.obj", "bunny");
		bunny = CreateEntity();
		bunny.AddComponent<LineRendererComponent>(AssetManager::GetVertexNormalLineMesh("bunny"));
		bunny.AddComponent<MeshRendererComponent>(AssetManager::GetFaceNormalMesh("bunny"));
	};
	float ie = 0;
	virtual void Update() 
	{
		vertex_array->Bind();
		auto& trans = bunny.GetComponent<TransformComponent>();
		ie += 0.02;
		trans.Rotation = { 0, ie, 0 };


		line_shader->Use();
		auto LineMeshes = GetRegistry().view<TransformComponent, LineRendererComponent>();
		for (auto& entity : LineMeshes)
		{
			auto [TransformComp, LineRendererComp] = LineMeshes.get<TransformComponent, LineRendererComponent>(entity);
			line_shader->SetMat4("model", TransformComp.GetTransform());
			for(auto buffer:LineRendererComp.mesh->GetBuffers())
			{
				vertex_array->AttachBuffer(*buffer);
			}

			glDrawArrays(LineRendererComp.mesh->GetGLDrawType(), 0, LineRendererComp.mesh->GetVertices().size());
		}
		light_shader->Use();
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
	};
	virtual void LateUpdate()
	{
		ImGui::ShowDemoWindow();
	}
	virtual void OnDisable() {};
	virtual void OnDestroy() 
	{
		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);
	};
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