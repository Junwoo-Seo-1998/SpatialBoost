#include "DebugRenderer.h"

#include <glm/ext/matrix_transform.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "Core/Utils/MeshGenerator.h"

std::shared_ptr<VertexArray> DebugRenderer::DebugVertexArray;
std::shared_ptr<Shader> DebugRenderer::DebugShader;
std::shared_ptr<VertexBuffer> DebugRenderer::DebugBoxBuffer;
std::shared_ptr<IndexBuffer> DebugRenderer::DebugBoxIndexBuffer;

std::shared_ptr<VertexBuffer> DebugRenderer::DebugSphereBuffer;
std::shared_ptr<IndexBuffer> DebugRenderer::DebugSphereIndexBuffer;

unsigned int DebugRenderer::NumOfIndicies = 0;

void DebugRenderer::DrawBegin(const glm::mat4& view, const glm::mat4& projection)
{
	DebugVertexArray->Bind();
	DebugShader->Use();
	DebugShader->TrySetMat4("Matrix.View", view);
	DebugShader->TrySetMat4("Matrix.Projection", projection);
}

void DebugRenderer::DrawEnd()
{
	DebugVertexArray->UnBind();
}

void DebugRenderer::DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color)
{
	glm::vec3 center = (min + max) / 2.f;
	glm::vec3 size = max - min;
	glm::mat4 model = glm::translate(glm::mat4(1.f), center) * glm::scale(glm::mat4(1.f), size);

	DebugShader->TrySetMat4("Matrix.Model", model);
	DebugShader->TrySetFloat3("BaseColor", color);
	DebugBoxBuffer->BindToVertexArray();
	DebugBoxIndexBuffer->BindToVertexArray();

	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, nullptr);
}

void DebugRenderer::DrawSphere(const glm::vec3& position, float radius, const glm::vec3& color)
{
	glm::mat4 model = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), glm::vec3{ radius });

	DebugShader->TrySetMat4("Matrix.Model", model);
	DebugShader->TrySetFloat3("BaseColor", color);
	DebugSphereBuffer->BindToVertexArray();
	DebugSphereIndexBuffer->BindToVertexArray();

	glDrawElements(GL_LINE_STRIP, NumOfIndicies, GL_UNSIGNED_INT, nullptr);
}

void DebugRenderer::Init()
{
	DebugVertexArray = VertexArray::CreateVertexArray();
	const char* vert=R"(
		#version 460 core
		struct MatrixData
		{
		    mat4 Model;
		    mat4 View;
		    mat4 Projection;
		};

		layout (location = 0) in vec3 aPos;
		uniform MatrixData Matrix;
		void main()
		{
			gl_Position = Matrix.Projection*Matrix.View*Matrix.Model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)";
	const char* frag = R"(
		#version 460 core
		out vec3 FragColor;
		uniform vec3 BaseColor;
		void main()
		{
			FragColor = BaseColor;
		} 
	)";
	DebugShader = Shader::CreateShaderFromString({
		{ShaderType::VertexShader,{vert}},
		{ShaderType::FragmentShader,{frag}},
	});

	const float box_vertices[] = {
		// Front face
		-0.5f, -0.5f,  0.5f, // bottom-left
		 0.5f, -0.5f,  0.5f, // bottom-right
		 0.5f,  0.5f,  0.5f, // top-right
		-0.5f,  0.5f,  0.5f, // top-left

		// Back face
		-0.5f, -0.5f, -0.5f, // bottom-left
		 0.5f, -0.5f, -0.5f, // bottom-right
		 0.5f,  0.5f, -0.5f, // top-right
		-0.5f,  0.5f, -0.5f  // top-left
	};
	const unsigned int box_indices[] = {
		// Front face
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		// Back face
		4, 5,
		5, 6,
		6, 7,
		7, 4,

		// Connections between front and back faces
		0, 4,
		1, 5,
		2, 6,
		3, 7
	};

	DebugBoxBuffer = VertexBuffer::CreateVertexBuffer(sizeof(box_vertices));
	DebugBoxBuffer->SetData(sizeof(box_vertices), box_vertices);
	DebugBoxBuffer->SetDataTypes({ {0, DataType::Float3} });
	DebugBoxIndexBuffer = IndexBuffer::CreateIndexBuffer(sizeof(box_indices));
	DebugBoxIndexBuffer->SetData(sizeof(box_indices), box_indices);

	auto [gen_points, gen_indices] = MeshGenerator::GenerateSpherePointsWithIndices(1.f, 30, 30);
	DebugSphereBuffer = VertexBuffer::CreateVertexBuffer(gen_points->size() * sizeof(glm::vec3));
	DebugSphereBuffer->SetData(gen_points->size() * sizeof(glm::vec3), gen_points->data());
	DebugSphereBuffer->SetDataTypes({ {0, DataType::Float3} });
	DebugSphereIndexBuffer = IndexBuffer::CreateIndexBuffer(gen_indices->size() * sizeof(unsigned int));
	DebugSphereIndexBuffer->SetData(gen_indices->size() * sizeof(unsigned int), gen_indices->data());

	NumOfIndicies = gen_indices->size();
}

void DebugRenderer::Shutdown()
{
}
