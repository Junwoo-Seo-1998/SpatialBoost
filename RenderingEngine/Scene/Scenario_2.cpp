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
#include "Scenario_2.h"

#include <iostream>
#include <glm/ext/scalar_constants.hpp>

#include "imgui.h"
#include "CommonOverlay/CommonOverlay.h"
#include "Core/Application.h"
#include "Core/AssetManager.h"
#include "Core/Component/LightComponent.h"
#include "Core/Component/MaterialComponent.h"
#include "Core/Component/RendererComponent.h"
#include "Core/Component/TransformComponent.h"
#include "Core/Event/ApplicationEvents/ApplicationEvents.h"
#include "Core/Utils/MeshGenerator.h"
#include "GLFW/glfw3.h"
#include "Core/Graphics/Shader.h"
#include "Core/Graphics/VertexArray.h"
#include "Core/Layer/LayerManager.h"

Scenario_2::Scenario_2(Application& app)
	: Scene(app)
{
}

void Scenario_2::Awake()
{
	overlay = std::make_shared<CommonOverlay>();
	Application::Get().GetLayerManager()->PushOverlay(overlay);
}

void Scenario_2::OnEnable()
{
}

void Scenario_2::Start()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	line_shader = AssetManager::GetShader("line_shader");
	selected_shader = "Phong_Shading";
	current_shader = AssetManager::GetShader(selected_shader);

	vertex_array = std::make_shared<VertexArray>();
	vertex_array->Bind();

	demo_mesh = CreateEntity();
	demo_mesh.GetComponent<TransformComponent>().Scale = { 1,1,1 };
	demo_mesh.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh("bunny"));
	demo_mesh.AddComponent<FaceNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("bunny"));
	demo_mesh.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh("bunny"));
	demo_mesh.AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetVertexNormalMesh("bunny"));
	auto mat = demo_mesh.AddComponent<MaterialComponent>();

	float radius = 3.f;
	orbit = CreateEntity();
	auto& parent_transform = orbit.GetComponent<TransformComponent>();
	orbit.AddComponent<LineRendererComponent>(MeshGenerator::GenerateOrbit(radius));
	float d_theta = 2.f * glm::pi<float>() / static_cast<float>(16);
	float theta = 0.f;
	for (int step = 0; step < 16; ++step)
	{
		glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
		Lights[step] = CreateEntity();
		Lights[step].GetComponent<TransformComponent>().Position = position;
		Lights[step].GetComponent<TransformComponent>().Parent = &parent_transform;
		Lights[step].AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh("GeneratedOrbitSphere"));
		Lights[step].AddComponent<FaceNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("GeneratedOrbitSphere"));
		Lights[step].AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh("GeneratedOrbitSphere"));
		Lights[step].AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetVertexNormalMesh("GeneratedOrbitSphere"));

		auto& Light = Lights[step].AddComponent<LightComponent>();
		Light.light.m_LightType = LightType::PointLight;
		glm::vec3 random{ (glm::cos(step) + 1.f) / 2.f,(glm::sin(step) + 1.f) / 2.f, (1.f) / 2.f };
		Light.light.Diffuse = random;
		Light.light.Ambient = random;
		Light.light.Specular = random;
		
		theta += d_theta;
	}

	FrameBufferSpecification specification{ 1024,1024, {FrameBufferFormat::RGBA, FrameBufferFormat::Depth} };
	for (int i = 0; i < 6; ++i)
	{
		FrameBuffers[i] = FrameBuffer::CreateFrameBuffer(specification);
	}
}

void Scenario_2::DrawEnv(const glm::mat4& worldToCam)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world_to_cam = worldToCam;
	vertex_array->Bind();
	perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(90.f), 1.f, 0.1f, 1000.f);
	glDisable(GL_DEPTH_TEST);
	auto skybox = AssetManager::GetShader("skybox_shader");
	skybox->Use();
	vertex_array->AttachBuffer(*AssetManager::GetSkybox());
	skybox->SetMat4("view", glm::mat3{ world_to_cam });
	skybox->SetMat4("projection", perspective);
	skybox->SetTexture("skybox[0]", AssetManager::GetTexture("sky_left"), 0);
	skybox->SetTexture("skybox[1]", AssetManager::GetTexture("sky_right"), 1);

	skybox->SetTexture("skybox[2]", AssetManager::GetTexture("sky_front"), 2);
	skybox->SetTexture("skybox[3]", AssetManager::GetTexture("sky_back"), 3);

	skybox->SetTexture("skybox[4]", AssetManager::GetTexture("sky_bottom"), 4);
	skybox->SetTexture("skybox[5]", AssetManager::GetTexture("sky_top"), 5);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	current_shader = AssetManager::GetShader(selected_shader);

	float time = (float)glfwGetTime();
	float dt = time - m_LastTime;
	m_LastTime = time;

	//for gui control
	if (cullBackFace)
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

	if (!StopRotation)
	{
		auto& demo_trans = demo_mesh.GetComponent<TransformComponent>();
		demo_trans.Rotation.y += demo_trans.Rotation.y >= 360.f ? -360.f + m_center_speed * dt : m_center_speed * dt;
		auto& orbit_trans = orbit.GetComponent<TransformComponent>();
		orbit_trans.Rotation.y += orbit_trans.Rotation.y >= 360.f ? -360.f + m_orbit_speed * dt : m_orbit_speed * dt;
	}



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




	{//light update
		float radius = 3.f;
		float d_theta = 2.f * glm::pi<float>() / static_cast<float>(light_number);
		float theta = 0.f;
		auto Lights = GetRegistry().view<TransformComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Lights)
		{
			if (i == light_number)
				break;
			glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
			auto [TransformComp, Light] = Lights.get<TransformComponent, LightComponent>(entity);
			TransformComp.Position = position;
			glm::vec3 lightPos = glm::vec3(TransformComp.GetTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
			if (Light.light.m_LightType == LightType::SpotLight)
			{
				Light.light.direction = glm::vec3{ 0, -100, 0 } - lightPos;
			}
			Light.light.position = lightPos;
			theta += d_theta;
			i++;
		}
	}




	{
		//draw light
		auto light_shader = AssetManager::GetShader("light_shader");
		light_shader->Use();
		light_shader->SetMat4("Matrix.View", world_to_cam);
		light_shader->SetMat4("Matrix.Projection", perspective);
		auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Meshes)
		{
			if (i == light_number)
				break;
			auto [TransformComp, MeshRendererComp, LightComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent, LightComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			light_shader->SetMat4("Matrix.Model", model);
			vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());
			light_shader->SetFloat4("BaseColor", glm::vec4(LightComp.light.Diffuse, 1.f));
			if (MeshRendererComp.mesh->GetUseIndex())
			{
				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
				glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), static_cast<GLsizei>(MeshRendererComp.mesh->GetIndices()->size()), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(MeshRendererComp.mesh->GetVertices()->size()));
			}
			i++;
		}
	}




	current_shader->Use();
	current_shader->SetMat4("Matrix.View", world_to_cam);
	current_shader->SetMat4("Matrix.Projection", perspective);
	{
		auto Lights = GetRegistry().view<TransformComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Lights)
		{
			if (i == light_number)
				break;
			auto [TransformComp, Light] = Lights.get<TransformComponent, LightComponent>(entity);
			std::string to_string = std::to_string(i);
			current_shader->SetInt("Light[" + to_string + "].LightType", static_cast<int>(light_type));
			glm::vec3 lightPos = glm::vec3(TransformComp.GetTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
			current_shader->SetFloat3("Light[" + to_string + "].Position", lightPos);
			current_shader->SetFloat3("Light[" + to_string + "].Direction", glm::vec3{ 0, -1, 0 } - lightPos);
			current_shader->SetFloat("Light[" + to_string + "].InnerAngle", glm::radians(Light.light.m_Angle.inner));
			current_shader->SetFloat("Light[" + to_string + "].OuterAngle", glm::radians(Light.light.m_Angle.outer));
			current_shader->SetFloat("Light[" + to_string + "].FallOff", Light.light.falloff);

			current_shader->SetFloat3("Light[" + to_string + "].Ambient", Light.light.Ambient);
			current_shader->SetFloat3("Light[" + to_string + "].Diffuse", Light.light.Diffuse);
			current_shader->SetFloat3("Light[" + to_string + "].Specular", Light.light.Specular);
			i++;
		}
	}
	current_shader->SetInt("LightNumbers", light_number);
	current_shader->SetFloat3("CameraPosition", campos);
	current_shader->SetFloat3("Material.Ambient", Mat_Ambient);
	current_shader->SetFloat3("Material.Diffuse", { 1.0f / 16.f,1.0f / 16.f,1.0f / 16.f });
	current_shader->SetFloat3("Material.Specular", { 0.5f,0.5f,0.5f });
	current_shader->SetFloat3("Material.Emissive", Mat_Emissive);
	current_shader->SetFloat("Material.Shininess", 32.f);

	current_shader->SetInt("UVType", current_uv_method);

	current_shader->SetTexture("DiffuseTexture", AssetManager::GetTexture("diff"), 0);
	current_shader->SetTexture("SpecularTexture", AssetManager::GetTexture("spec"), 1);

	current_shader->SetFloat3("globalAmbient", Global_Ambient_Color);
	current_shader->SetFloat("Attenuation.c1", c1);
	current_shader->SetFloat("Attenuation.c2", c2);
	current_shader->SetFloat("Attenuation.c3", c3);

	current_shader->SetFloat("Fog.Near", fog_near);
	current_shader->SetFloat("Fog.Far", fog_far);
	current_shader->SetFloat3("Fog.Color", fog_color);

	//Vert normal 
	{
		current_shader->SetInt("useTexture", 0);
		current_shader->SetInt("showReflect", 0);
		current_shader->SetInt("showRefract", 0);
		auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent>(entt::exclude<MaterialComponent, LightComponent>);
		for (auto& entity : Meshes)
		{
			auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			current_shader->SetMat4("Matrix.Model", model);
			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
			current_shader->SetMat4("Matrix.Normal", normal_matrix);
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

void Scenario_2::UpdateFrameBuffers(const glm::vec3& object_pos)
{
	glViewport(0, 0, 1024, 1024);
	FrameBuffers[0]->Bind();
	//left
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { -1,0,0 }));
	FrameBuffers[1]->Bind();
	//right
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { 1,0,0 }));

	FrameBuffers[2]->Bind();
	//front
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { 0,0,-1 }));
	FrameBuffers[3]->Bind();
	//back
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { 0,0,1 }));

	FrameBuffers[4]->Bind();
	//bottom
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { 0,-1,0 }, { 0,0,-1 }));
	FrameBuffers[5]->Bind();
	//top
	DrawEnv(Math::BuildCameraMatrixWithDirection(object_pos, { 0,1,0 }, { 0,0,1 }));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scenario_2::Update()
{
	float time = (float)glfwGetTime();
	float dt = time - m_LastTime;
	m_LastTime = time;
	reDrawAcc += dt;
	if (reDrawTime <= reDrawAcc)
	{
		reDrawAcc = 0.f;
		UpdateFrameBuffers({ 0,0,0 });
	}

	world_to_cam = Math::BuildCameraMatrix(campos, { 0,0,0 }, { 0,1,0 });
	vertex_array->Bind();
	auto [width, height] = Application::Get().GetWindowSize();
	float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
	glViewport(0, 0, width, height);
	perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), AspectRatio, 0.1f, 1000.f);
	glDisable(GL_DEPTH_TEST);
	auto skybox = AssetManager::GetShader("skybox_shader");
	skybox->Use();
	vertex_array->AttachBuffer(*AssetManager::GetSkybox());
	skybox->SetMat4("view", glm::mat3{ world_to_cam });
	skybox->SetMat4("projection", perspective);
	skybox->SetTexture("skybox[0]", AssetManager::GetTexture("sky_left"), 0);
	skybox->SetTexture("skybox[1]", AssetManager::GetTexture("sky_right"), 1);

	skybox->SetTexture("skybox[2]", AssetManager::GetTexture("sky_front"), 2);
	skybox->SetTexture("skybox[3]", AssetManager::GetTexture("sky_back"), 3);

	skybox->SetTexture("skybox[4]", AssetManager::GetTexture("sky_bottom"), 4);
	skybox->SetTexture("skybox[5]", AssetManager::GetTexture("sky_top"), 5);


	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);

	current_shader = AssetManager::GetShader(selected_shader);


	//for gui control
	if (cullBackFace)
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

	if (!StopRotation)
	{
		auto& demo_trans = demo_mesh.GetComponent<TransformComponent>();
		demo_trans.Rotation.y += demo_trans.Rotation.y >= 360.f ? -360.f + m_center_speed * dt : m_center_speed * dt;
		auto& orbit_trans = orbit.GetComponent<TransformComponent>();
		orbit_trans.Rotation.y += orbit_trans.Rotation.y >= 360.f ? -360.f + m_orbit_speed * dt : m_orbit_speed * dt;
	}



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




	{//light update
		float radius = 3.f;
		float d_theta = 2.f * glm::pi<float>() / static_cast<float>(light_number);
		float theta = 0.f;
		auto Lights = GetRegistry().view<TransformComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Lights)
		{
			if (i == light_number)
				break;
			glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
			auto [TransformComp, Light] = Lights.get<TransformComponent, LightComponent>(entity);
			TransformComp.Position = position;
			glm::vec3 lightPos = glm::vec3(TransformComp.GetTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
			if (Light.light.m_LightType == LightType::SpotLight)
			{
				Light.light.direction = glm::vec3{ 0, -100, 0 } - lightPos;
			}
			Light.light.position = lightPos;
			theta += d_theta;
			i++;
		}
	}




	{
		//draw light
		auto light_shader = AssetManager::GetShader("light_shader");
		light_shader->Use();
		light_shader->SetMat4("Matrix.View", world_to_cam);
		light_shader->SetMat4("Matrix.Projection", perspective);
		auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Meshes)
		{
			if (i == light_number)
				break;
			auto [TransformComp, MeshRendererComp, LightComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent, LightComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			light_shader->SetMat4("Matrix.Model", model);
			vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());
			light_shader->SetFloat4("BaseColor", glm::vec4(LightComp.light.Diffuse, 1.f));
			if (MeshRendererComp.mesh->GetUseIndex())
			{
				vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetIndexBuffer());
				glDrawElements(MeshRendererComp.mesh->GetGLDrawType(), static_cast<GLsizei>(MeshRendererComp.mesh->GetIndices()->size()), GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawArrays(MeshRendererComp.mesh->GetGLDrawType(), 0, static_cast<GLsizei>(MeshRendererComp.mesh->GetVertices()->size()));
			}
			i++;
		}
	}




	current_shader->Use();
	current_shader->SetMat4("Matrix.View", world_to_cam);
	current_shader->SetMat4("Matrix.Projection", perspective);
	{
		auto Lights = GetRegistry().view<TransformComponent, LightComponent>();
		int i = 0;
		for (auto& entity : Lights)
		{
			if (i == light_number)
				break;
			auto [TransformComp, Light] = Lights.get<TransformComponent, LightComponent>(entity);
			std::string to_string = std::to_string(i);
			current_shader->SetInt("Light[" + to_string + "].LightType", static_cast<int>(light_type));
			glm::vec3 lightPos = glm::vec3(TransformComp.GetTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
			current_shader->SetFloat3("Light[" + to_string + "].Position", lightPos);
			current_shader->SetFloat3("Light[" + to_string + "].Direction", glm::vec3{ 0, -1, 0 } - lightPos);
			current_shader->SetFloat("Light[" + to_string + "].InnerAngle", glm::radians(Light.light.m_Angle.inner));
			current_shader->SetFloat("Light[" + to_string + "].OuterAngle", glm::radians(Light.light.m_Angle.outer));
			current_shader->SetFloat("Light[" + to_string + "].FallOff", Light.light.falloff);

			current_shader->SetFloat3("Light[" + to_string + "].Ambient", Light.light.Ambient);
			current_shader->SetFloat3("Light[" + to_string + "].Diffuse", Light.light.Diffuse);
			current_shader->SetFloat3("Light[" + to_string + "].Specular", Light.light.Specular);
			i++;
		}
	}
	current_shader->SetInt("LightNumbers", light_number);
	current_shader->SetFloat3("CameraPosition", campos);
	current_shader->SetFloat3("Material.Ambient", Mat_Ambient);
	current_shader->SetFloat3("Material.Diffuse", { 1.0f / 16.f,1.0f / 16.f,1.0f / 16.f });
	current_shader->SetFloat3("Material.Specular", { 0.5f,0.5f,0.5f });
	current_shader->SetFloat3("Material.Emissive", Mat_Emissive);
	current_shader->SetFloat("Material.Shininess", 32.f);

	current_shader->SetInt("UVType", current_uv_method);

	current_shader->SetTexture("DiffuseTexture", AssetManager::GetTexture("diff"), 0);
	current_shader->SetTexture("SpecularTexture", AssetManager::GetTexture("spec"), 1);

	current_shader->SetFloat3("globalAmbient", Global_Ambient_Color);
	current_shader->SetFloat("Attenuation.c1", c1);
	current_shader->SetFloat("Attenuation.c2", c2);
	current_shader->SetFloat("Attenuation.c3", c3);

	current_shader->SetFloat("Fog.Near", fog_near);
	current_shader->SetFloat("Fog.Far", fog_far);
	current_shader->SetFloat3("Fog.Color", fog_color);

	//Vert normal 
	{
		current_shader->SetInt("useTexture", 0);
		current_shader->SetInt("showReflect", 0);
		current_shader->SetInt("showRefract", 0);
		auto Meshes = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent>(entt::exclude<MaterialComponent, LightComponent>);
		for (auto& entity : Meshes)
		{
			auto [TransformComp, MeshRendererComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			current_shader->SetMat4("Matrix.Model", model);
			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
			current_shader->SetMat4("Matrix.Normal", normal_matrix);
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

		current_shader->SetInt("useTexture", 1);
		if (useCpu)
			current_shader->SetInt("UseCPU", 1);
		else
			current_shader->SetInt("UseCPU", 0);

		current_shader->SetInt("NormalEntity", TextureEntity);

		current_shader->SetFloat("Nt", Mat_RefractiveIndex);
		current_shader->SetFloat("RGBRatio", Mat_RGBRefractionRatio);
		current_shader->SetFloat("fresnelPower", Mat_FresnelPower);

		current_shader->SetTexture("skybox[0]", FrameBuffers[0]->GetColorTexture(), 0);
		current_shader->SetTexture("skybox[1]", FrameBuffers[1]->GetColorTexture(), 1);

		current_shader->SetTexture("skybox[2]", FrameBuffers[2]->GetColorTexture(), 2);
		current_shader->SetTexture("skybox[3]", FrameBuffers[3]->GetColorTexture(), 3);

		current_shader->SetTexture("skybox[4]", FrameBuffers[4]->GetColorTexture(), 4);
		current_shader->SetTexture("skybox[5]", FrameBuffers[5]->GetColorTexture(), 5);

		if (MatRadio == static_cast<int>(select::ShowFresnelEffect))
		{
			current_shader->SetInt("showReflect", 1);
			current_shader->SetInt("showRefract", 1);
		}
		else if (MatRadio == static_cast<int>(select::ShowReflection))
		{
			current_shader->SetInt("showReflect", 1);
			current_shader->SetInt("showRefract", 0);
		}
		else if (MatRadio == static_cast<int>(select::ShowRefraction))
		{
			current_shader->SetInt("showReflect", 0);
			current_shader->SetInt("showRefract", 1);
		}
		else
		{
			current_shader->SetInt("showReflect", 0);
			current_shader->SetInt("showRefract", 0);
			current_shader->SetTexture("DiffuseTexture", AssetManager::GetTexture("diff"), 0);
			current_shader->SetTexture("SpecularTexture", AssetManager::GetTexture("spec"), 1);
		}

		auto MeshesWithMaterial = GetRegistry().view<TransformComponent, VertexNormalMeshRendererComponent, MaterialComponent>();
		for (auto& entity : MeshesWithMaterial)
		{
			auto [TransformComp, MeshRendererComp, MaterialComp] = MeshesWithMaterial.get<TransformComponent, VertexNormalMeshRendererComponent, MaterialComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			current_shader->SetMat4("Matrix.Model", model);
			glm::mat4 normal_matrix = glm::transpose(glm::inverse(model));
			current_shader->SetMat4("Matrix.Normal", normal_matrix);
			vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());

			std::shared_ptr<BoundingBox> box = MeshRendererComp.mesh->GetBoundingBox();
			current_shader->SetFloat3("BoundingBox.Min", box->min);
			current_shader->SetFloat3("BoundingBox.Max", box->max);
			current_shader->SetFloat3("BoundingBox.Center", box->center);
			if (MeshRendererComp.mesh->HasUV())
			{
				if (TextureEntity == 0)
					vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetUV()->GetPointUVBuffer(static_cast<UVTypes>(current_uv_method)));
				else
					vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetUV()->GetNormalUVBuffer(static_cast<UVTypes>(current_uv_method)));
			}
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

void Scenario_2::LateUpdate()
{

	ImGui::Begin("Control");
	ImGui::SliderFloat3("cam position", &campos[0], -10.f, 10.f);
	const char* items[] =
	{
		"4Sphere", "bunny", "cube", "sphere", "sphere_modified","GeneratedSphere"
	};
	ImGui::Text("Mesh");
	if (ImGui::BeginCombo("##combo", current_mesh.c_str()))
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
	ImGui::Checkbox("Pause Rotation", &StopRotation);
	ImGui::DragInt("Light Numbers", &light_number, 1, 1, 16);

	ImGui::ColorEdit3("Global Ambient", &Global_Ambient_Color[0]);
	ImGui::DragFloat("Attenuation.c1", &c1, 0.1f, 0.f, 1.f);
	ImGui::DragFloat("Attenuation.c2", &c2, 0.1f, 0.f, 1.f);
	ImGui::DragFloat("Attenuation.c3", &c3, 0.1f, 0.f, 1.f);


	ImGui::Text("Material");
	ImGui::DragFloat3("Material Ambient", &Mat_Ambient[0], 0.1f, 0.001f, 1.f);
	ImGui::DragFloat3("Material Emissive", &Mat_Emissive[0], 0.1f, 0.001f, 1.f);
	ImGui::RadioButton("Show Fresnel Effect", &MatRadio, static_cast<int>(select::ShowFresnelEffect)); ImGui::SameLine();
	ImGui::RadioButton("Show Reflection", &MatRadio, static_cast<int>(select::ShowReflection));
	ImGui::RadioButton("Show Refraction", &MatRadio, static_cast<int>(select::ShowRefraction)); ImGui::SameLine();
	ImGui::RadioButton("None", &MatRadio, static_cast<int>(select::None));

	static const char* listbox_items[] = { "Air", "Hydrogen", "Water", "Olive Oil", "Ice (solidified water)", "Quartz", "Diamond", "Acrylic / plexiglas / Lucite" };
	static float listbox_values[] = { 1.000293f, 1.000132f, 1.333f, 1.47f, 1.31f, 1.46f, 2.42f, 1.49f };
	static int listbox_item_current = 1;
	ImGui::ListBox("Refractive Type", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

	if (ImGui::Button("Apply Selected Value"))
	{
		Mat_RefractiveIndex = listbox_values[listbox_item_current];
	}

	ImGui::DragFloat("Refractive Index", &Mat_RefractiveIndex, 0.1f, 1.0f, 100.f);
	ImGui::DragFloat("Refractive Index RGB Diff", &Mat_RGBRefractionRatio, 0.01f, 0.0f, 1.f);
	ImGui::DragFloat("Fresnel Power", &Mat_FresnelPower, 0.1f, 0.1f, 50.f);


	ImGui::Text("Light Settings");

	ImGui::RadioButton("PointLight", &light_type, static_cast<int>(LightType::PointLight)); ImGui::SameLine();
	ImGui::RadioButton("DirectionLight", &light_type, static_cast<int>(LightType::DirectionLight)); ImGui::SameLine();
	ImGui::RadioButton("SpotLight", &light_type, static_cast<int>(LightType::SpotLight));

	{
		ImGui::DragInt("Select Light", &current_light, 1, 1, 16);
		auto& light_comp = Lights[current_light - 1].GetComponent<LightComponent>();

		ImGui::ColorEdit4("Ambient Color", &light_comp.light.Ambient[0]);
		ImGui::ColorEdit4("Diffuse Color", &light_comp.light.Diffuse[0]);
		ImGui::ColorEdit4("Specular Color", &light_comp.light.Specular[0]);

		switch (light_type)
		{
		case 0:

			break;
		case 1:

			break;
		case 2:
			ImGui::DragFloat("InnerAngle", &light_comp.light.m_Angle.inner, 1.f, 0.f, light_comp.light.m_Angle.outer - 1.f);
			ImGui::DragFloat("OuterAngle", &light_comp.light.m_Angle.outer, 1.f, light_comp.light.m_Angle.inner + 1.f, 360.f);
			ImGui::DragFloat("FallOff", &light_comp.light.falloff, 1.f, 1.f, 100.f);
			break;
		}
	}
	ImGui::ColorEdit4("Line Color", &line_color[0]);
	ImGui::ColorEdit3("Fog Color", &fog_color[0]);
	ImGui::DragFloat("Fog Near", &fog_near, 0.5f, 1.f, fog_far - 1.f);
	ImGui::DragFloat("Fog Far", &fog_far, 0.5f, fog_near + 1.f, 200.f);

	const char* shaders[] =
	{
		"Phong_Shading", "Blinn_Shading"
	};

	if (ImGui::BeginCombo("Shaders", selected_shader.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(shaders); n++)
		{
			bool is_selected = (selected_shader == shaders[n]);
			if (ImGui::Selectable(shaders[n], is_selected))
				selected_shader = shaders[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Reload"))
	{
		if (selected_shader == "Phong_Shading")
		{
			current_shader = AssetManager::ReloadSherFromFile("Phong_Shading", "Assets/Shaders/common.glsl", "Assets/Shaders/phong_shading.vert", "Assets/Shaders/phong_shading.frag");
		}
		if (selected_shader == "Blinn_Shading")
		{
			current_shader = AssetManager::ReloadSherFromFile("Blinn_Shading", "Assets/Shaders/common.glsl", "Assets/Shaders/blinn_shading.vert", "Assets/Shaders/blinn_shading.frag");
		}
	}
	ImGui::Text("UV Control");
	ImGui::Checkbox("Use CPU", &useCpu);
	ImGui::RadioButton("Planar", &current_uv_method, static_cast<int>(UVTypes::Planar)); ImGui::SameLine();
	ImGui::RadioButton("Spherical", &current_uv_method, static_cast<int>(UVTypes::Spherical)); ImGui::SameLine();
	ImGui::RadioButton("Cylindrical", &current_uv_method, static_cast<int>(UVTypes::Cylindrical)); ImGui::SameLine();
	ImGui::RadioButton("CubeMap", &current_uv_method, static_cast<int>(UVTypes::CubeMap));

	ImGui::Text("Entity");
	ImGui::RadioButton("Point", &TextureEntity, static_cast<int>(0)); ImGui::SameLine();
	ImGui::RadioButton("Normal", &TextureEntity, static_cast<int>(1));

	ImGui::End();


	ImGui::Begin("FrameBuffers");
	constexpr ImVec2 size{ 100,100 };

	unsigned textureID = FrameBuffers[5]->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	textureID = FrameBuffers[0]->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = FrameBuffers[2]->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::SameLine();
	textureID = FrameBuffers[1]->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	textureID = FrameBuffers[3]->GetColorTexture(0)->GetTextureID();
	ImGui::SameLine();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	textureID = FrameBuffers[4]->GetColorTexture(0)->GetTextureID();
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(textureID)), size, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();
}

void Scenario_2::OnDisable()
{
}

void Scenario_2::OnDestroy()
{
	Application::Get().GetLayerManager()->PopOverlay(overlay);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glBindVertexArray(0);

}

void Scenario_2::OnEvent(Event& event)
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
