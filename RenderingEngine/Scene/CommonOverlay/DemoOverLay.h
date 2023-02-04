#pragma once
#include <string>

#include "Core/Layer/Layer.h"
class Event;
class DemoOverLay :public Layer
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
	std::string current_mesh;
};