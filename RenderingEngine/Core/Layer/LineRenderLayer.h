#pragma once
#include <memory>
#include <glm/vec4.hpp>

#include "Layer.h"
#include "Scene/DemoEnums/DemoEnums.h"
class VertexArray;

class LineRenderLayer:public Layer
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
	bool drawNormal = false;
	int radio = static_cast<int>(select::DrawVertexNormal);
	glm::vec4 lineColor = { 0.f,0.5f,2.f,1.f };
};
