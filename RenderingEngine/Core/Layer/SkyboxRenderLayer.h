#pragma once
#include "Layer.h"
#include "Core/AssetManager.h"
#include "Core/Graphics/VertexArray.h"

class SkyboxRenderLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnPostRender() override;
	void OnGuiRender() override;
	void OnEvent(Event& event) override;
private:
	std::shared_ptr<Shader> skybox_shader;
	std::shared_ptr<VertexArray> vao;
	bool drawSkyBox = true;
};
