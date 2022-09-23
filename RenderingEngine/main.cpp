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
#include"Core/Graphics/Buffer.h"
#include"Core/Graphics/VertexArray.h"
class MyScene : public Scene
{
	std::shared_ptr<VertexArray> vertex_array;
	std::shared_ptr<VertexBuffer> vertex_buffer_class;
	std::shared_ptr<ElementBuffer> element_buffer;
	std::shared_ptr<Shader> shader;

	Mesh mesh;

	const char* vertexShaderSource = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		uniform vec3 CamPos;
		out vec3 Normal;
		out vec3 LightPos;
		out vec3 FragPos;
		void main()
		{
			gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
			Normal=aNormal;
			LightPos=CamPos;
			FragPos=vec3(model*vec4(aPos,1.0));
		}
		)";
	const char* fragShaderSource = R"(
		#version 460 core
		in vec3 Normal;
		in vec3 LightPos;
		in vec3 FragPos;

		out vec4 FragColor;
		void main()
		{
			vec3 LightDir=normalize(LightPos-FragPos);
			float diff=max(dot(Normal,LightDir),0.f);
			vec3 diffuse=diff*vec3(1.f, 1.f, 1.f);
			//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);

			float ambientStrength=0.1;
			vec3 ambient = ambientStrength * vec3(1.f, 1.f, 1.f);
			vec3 result=(ambient+diffuse)*vec3(1.0f, 0.5f, 0.2f);
			FragColor = vec4(result, 1.0f);
		} 
		)";

	const char* DrawNormalVertexShaderSource = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		uniform mat4 mat;
		void main()
		{
			gl_Position = mat*vec4(aPos.x, aPos.y, aPos.z, 1.0);
			FragPos = vec3(mat*vec4(aPos.x, aPos.y, aPos.z, 1.0));
		}
		)";
	const char* DrawNormalFragShaderSource = R"(
		#version 460 core
		out vec4 FragColor;
		
		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		} 
		)";
	virtual void Awake() {};
	virtual void OnEnable() {};
	virtual void Start() 
	{
		ObjParser parser;
		//mesh = std::move(parser.LoadFileFast("../Assets/bunny_high_poly.obj"));
		//mesh=parser.LoadFile("../Assets/bunny_high_poly.obj");
		mesh = parser.LoadFileFast("../Assets/bunny_high_poly.obj");
		//mesh = parser.LoadFileFast("../Assets/4Sphere.obj");
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		auto obj_to_world = glm::mat4{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		auto world_to_cam = Math::BuildCameraMatrix({ 0,0,10 }, { 0,0,0 }, { 0,1,0 });
		auto perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), 800.f / 800.f, 0.1f, 100.f);
		
		shader = std::make_shared<Shader>(vertexShaderSource, fragShaderSource);
		shader->SetMat4("model", obj_to_world);
		shader->SetMat4("view", world_to_cam);
		shader->SetMat4("projection", perspective);
		shader->SetFloat3("CamPos", { 0,10,10 });

		vertex_array = std::make_shared<VertexArray>();
		vertex_array->Bind();
		vertex_buffer_class = std::make_shared<VertexBuffer>(mesh.GetVertices().size() * sizeof(Vertex));
		vertex_buffer_class->BufferData(mesh.GetVertices().data(), mesh.GetVertices().size() * sizeof(Vertex));
		vertex_buffer_class->DescribeData({ {0,Float3}, {1, Float3} });
		vertex_array->AttachBuffer(*vertex_buffer_class);
		element_buffer = std::make_shared<ElementBuffer>(mesh.GetFaces());
		vertex_array->AttachBuffer(*element_buffer);
	};
	virtual void Update() 
	{
		shader->Use();
		vertex_array->Bind();
		
		glDrawElements(GL_TRIANGLES, mesh.GetFaces().size()*3, GL_UNSIGNED_INT, nullptr);
	};
	virtual void LateUpdate() {};
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
		m_SceneManager->SetCurrentScene<MyScene>();
	}
};
std::shared_ptr<Application> CoreMain()
{
	std::cout<<"Vertex:" << sizeof(Vertex) << std::endl;
	return std::make_shared<MyApp>();
}