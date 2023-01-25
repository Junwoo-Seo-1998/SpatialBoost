#pragma once
#include "Layer.h"

class RenderLayer : public Layer
{
public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnGuiRender() override;
	void OnEvent(Event& event) override;
};
