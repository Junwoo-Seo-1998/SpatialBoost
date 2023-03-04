#pragma once
#include <memory>
#include <glm/glm.hpp>


class VertexArray;
class Application;
class Shader;
class VertexBuffer;
class IndexBuffer;

class DebugRenderer
{
	friend Application;
public:
	static void DrawBegin(const glm::mat4& view, const glm::mat4& projection);
	static void DrawEnd();
	static void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec3& color = { 0.5,0.5,0.5 });
	static void DrawSphere(const glm::vec3& position, float radius, const glm::vec3& color = { 0.5,0.5,0.5 });

private:
	static void Init();
	static void Shutdown();
private:
	static std::shared_ptr<VertexArray> DebugVertexArray;
	static std::shared_ptr<Shader> DebugShader;
	static std::shared_ptr<VertexBuffer> DebugBoxBuffer;
	static std::shared_ptr<IndexBuffer> DebugBoxIndexBuffer;

	static std::shared_ptr<VertexBuffer> DebugSphereBuffer;
	static std::shared_ptr<IndexBuffer> DebugSphereIndexBuffer;

	static unsigned int NumOfIndicies;
};
