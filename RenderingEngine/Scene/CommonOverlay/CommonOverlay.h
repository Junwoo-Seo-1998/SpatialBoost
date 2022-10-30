#pragma once
#include "Core/Layer/Layer.h"
class Event;
class CommonOverlay :public Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;
	virtual void OnGuiRender() override;
	virtual void OnEvent([[maybe_unused]] Event& event) override;
};
