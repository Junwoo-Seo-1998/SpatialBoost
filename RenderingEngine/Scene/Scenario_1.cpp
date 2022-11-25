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
#include "Scenario_1.h"

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

Scenario_1::Scenario_1(Application& app)
	: Scene(app)
{
}

void Scenario_1::Awake()
{
	overlay = std::make_shared<CommonOverlay>();
	Application::Get().GetLayerManager()->PushOverlay(overlay);
}

void Scenario_1::OnEnable()
{
}

void Scenario_1::Start()
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



	Entity plane = CreateEntity();
	plane.GetComponent<TransformComponent>().Position = { 0,-1,0 };
	plane.GetComponent<TransformComponent>().Scale = { 5.f,5.f,1.f };
	plane.GetComponent<TransformComponent>().Rotation = { glm::radians(-90.f),0.f,0.f };
	plane.AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("Plane"));

	float radius = 3.f;
	orbit = CreateEntity();
	auto& parent_transform = orbit.GetComponent<TransformComponent>();
	orbit.AddComponent<LineRendererComponent>(MeshGenerator::GenerateOrbit(radius));
	float d_theta = 2.f * glm::pi<float>() / static_cast<float>(16);
	float theta = 0.f;
	for (int step = 0; step < 16; ++step)
	{
		glm::vec3 position{ radius * glm::sin(theta), 0.f, radius * glm::cos(theta) };
		auto GeneratedSphere = CreateEntity();
		GeneratedSphere.GetComponent<TransformComponent>().Position = position;
		GeneratedSphere.GetComponent<TransformComponent>().Parent = &parent_transform;
		GeneratedSphere.AddComponent<FaceNormalLineRendererComponent>(AssetManager::GetFaceNormalLineMesh("GeneratedOrbitSphere"));
		GeneratedSphere.AddComponent<FaceNormalMeshRendererComponent>(AssetManager::GetFaceNormalMesh("GeneratedOrbitSphere"));
		GeneratedSphere.AddComponent<VertexNormalLineRendererComponent>(AssetManager::GetVertexNormalLineMesh("GeneratedOrbitSphere"));
		GeneratedSphere.AddComponent<VertexNormalMeshRendererComponent>(AssetManager::GetVertexNormalMesh("GeneratedOrbitSphere"));
		auto Light = GeneratedSphere.AddComponent<LightComponent>();
		Light.light.m_LightType = LightType::SpotLight;
		theta += d_theta;
	}
}

void Scenario_1::Update()
{
	vertex_array->Bind();
	auto [width, height] = Application::Get().GetWindowSize();
	float AspectRatio = static_cast<float>(width) / static_cast<float>(height);
	perspective = Math::BuildPerspectiveProjectionMatrixFovy(glm::radians(45.f), AspectRatio, 0.1f, 1000.f);
	glDepthMask(GL_FALSE);
	auto skybox = AssetManager::GetShader("skybox_shader");
	skybox->Use();
	vertex_array->AttachBuffer(*AssetManager::GetSkybox());
	skybox->SetMat4("view", glm::mat3{ world_to_cam });
	skybox->SetMat4("projection", perspective );
	skybox->SetTexture("skybox[0]", AssetManager::GetTexture("sky_left"), 0);
	skybox->SetTexture("skybox[1]", AssetManager::GetTexture("sky_right"), 1);

	skybox->SetTexture("skybox[2]", AssetManager::GetTexture("sky_front"), 2);
	skybox->SetTexture("skybox[3]", AssetManager::GetTexture("sky_back"), 3);

	skybox->SetTexture("skybox[5]", AssetManager::GetTexture("sky_top"), 4);
	skybox->SetTexture("skybox[4]", AssetManager::GetTexture("sky_bottom"), 5);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

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
			if(i==light_number)
				break;
			auto [TransformComp, MeshRendererComp, LightComp] = Meshes.get<TransformComponent, VertexNormalMeshRendererComponent, LightComponent>(entity);
			glm::mat4 model = TransformComp.GetTransform();
			light_shader->SetMat4("Matrix.Model", model);
			vertex_array->AttachBuffer(*MeshRendererComp.mesh->GetBuffer());
			light_shader->SetFloat4("BaseColor", Light_Diffuse_Color);
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
			current_shader->SetInt("Light[" + to_string + "].LightType", light_type);
			glm::vec3 lightPos = glm::vec3(TransformComp.GetTransform() * glm::vec4(0.f, 0.f, 0.f, 1.f));
			current_shader->SetFloat3("Light[" + to_string + "].Position", lightPos);
			current_shader->SetFloat3("Light[" + to_string + "].Direction", glm::vec3{ 0, -1, 0 } - lightPos);
			current_shader->SetFloat("Light[" + to_string + "].InnerAngle", glm::radians(inner));
			current_shader->SetFloat("Light[" + to_string + "].OuterAngle", glm::radians(outer));
			current_shader->SetFloat("Light[" + to_string + "].FallOff", falloff);

			current_shader->SetFloat3("Light[" + to_string + "].Ambient", Light_Ambient_Color);
			current_shader->SetFloat3("Light[" + to_string + "].Diffuse", Light_Diffuse_Color);
			current_shader->SetFloat3("Light[" + to_string + "].Specular", Light_Specular_Color);
			i++;
		}
	}

	current_shader->SetInt("LightNumbers", light_number);
	current_shader->SetFloat3("CameraPosition", { 0,2,5 });
	current_shader->SetFloat3("Material.Ambient", Mat_Ambient);
	current_shader->SetFloat3("Material.Diffuse", { 1.0f/16.f,1.0f / 16.f,1.0f / 16.f });
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
		if(useCpu)
			current_shader->SetInt("UseCPU", 1);
		else
			current_shader->SetInt("UseCPU", 0);

		current_shader->SetInt("NormalEntity", TextureEntity);

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
				if(TextureEntity==0)
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

void Scenario_1::LateUpdate()
{

	ImGui::Begin("Control");
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
	ImGui::DragFloat("Attenuation.c1", &c1,0.1f,0.f,1.f);
	ImGui::DragFloat("Attenuation.c2", &c2,0.1f,0.f,1.f);
	ImGui::DragFloat("Attenuation.c3", &c3,0.1f,0.f,1.f);


	ImGui::Text("Material");
	ImGui::DragFloat3("Material Ambient", &Mat_Ambient[0], 0.1f, 0.001f, 1.f);
	ImGui::DragFloat3("Material Emissive", &Mat_Emissive[0], 0.1f, 0.001f, 1.f);

	ImGui::Text("Light Type");

	
	ImGui::RadioButton("PointLight", &light_type, static_cast<int>(LightType::PointLight)); ImGui::SameLine();
	ImGui::RadioButton("DirectionLight", &light_type, static_cast<int>(LightType::DirectionLight)); ImGui::SameLine();
	ImGui::RadioButton("SpotLight", &light_type, static_cast<int>(LightType::SpotLight));

	ImGui::ColorEdit4("Ambient Color", &Light_Ambient_Color[0]);
	ImGui::ColorEdit4("Diffuse Color", &Light_Diffuse_Color[0]);
	ImGui::ColorEdit4("Specular Color", &Light_Specular_Color[0]);

	switch (light_type)
	{
	case 0:
		
		break;
	case 1:

		break;
	case 2:
		ImGui::DragFloat("InnerAngle", &inner, 1.f, 0.f, outer - 1.f);
		ImGui::DragFloat("OuterAngle", &outer, 1.f, inner + 1.f, 360.f);
		ImGui::DragFloat("FallOff", &falloff, 1.f, 1.f, 100.f);
		break;
	}

	ImGui::ColorEdit4("Line Color", &line_color[0]);
	const char* shaders[] =
	{
		"Phong_Shading","Phong_Lighting","Blinn_Shading"
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

	ImGui::ColorEdit3("Fog Color", &fog_color[0]);
	ImGui::DragFloat("Fog Near", &fog_near, 0.5f, 1.f, fog_far - 1.f);
	ImGui::DragFloat("Fog Far", &fog_far, 0.5f, fog_near + 1.f, 200.f);

	if(ImGui::Button("Reload"))
	{
		if(selected_shader== "Phong_Shading")
		{
			current_shader = AssetManager::ReloadSherFromFile("Phong_Shading", "Assets/Shaders/common.glsl", "Assets/Shaders/phong_shading.vert", "Assets/Shaders/phong_shading.frag");
		}
		if (selected_shader == "Phong_Lighting")
		{
			current_shader = AssetManager::ReloadSherFromFile("Phong_Lighting", "Assets/Shaders/common.glsl", "Assets/Shaders/phong_light.vert", "Assets/Shaders/phong_light.frag");
		}
		if(selected_shader == "Blinn_Shading")
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
}

void Scenario_1::OnDisable()
{
}

void Scenario_1::OnDestroy()
{
	Application::Get().GetLayerManager()->PopOverlay(overlay);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	glBindVertexArray(0);

}

void Scenario_1::OnEvent(Event& event)
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
