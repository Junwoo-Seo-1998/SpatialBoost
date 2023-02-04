#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
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
	void DeferredRender();
	void DeferredRenderQuad();
	void ForwardRender();
	
private:
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<VertexBuffer> quad_buffer;
	std::shared_ptr<FrameBuffer> deferred_fb;
	Material default_material{};
	bool copyDepthInfo = true;
	int width = 800;
	int height = 800;
};
