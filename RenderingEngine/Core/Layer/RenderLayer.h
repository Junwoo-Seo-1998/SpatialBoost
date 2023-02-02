#pragma once
#include "Layer.h"
#include <memory>

#include "Core/Data/Material.h"
class Texture;
class VertexBuffer;
class FrameBuffer;
class VertexArray;
class RenderLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
	void OnGuiRender() override;
	void OnEvent(Event& event) override;
private:
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<VertexBuffer> quad_buffer;
	std::shared_ptr<FrameBuffer> deferred_fb;
	Material default_material{};
	bool copyDepthInfo = true;
	int width = 800;
	int height = 800;
};
