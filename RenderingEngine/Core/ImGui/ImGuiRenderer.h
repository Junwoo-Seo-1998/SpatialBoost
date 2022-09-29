#pragma once
class ImGuiRenderer
{
public:
	ImGuiRenderer();
	~ImGuiRenderer() = default;
	void OnStart(void* window);
	void GuiBegin();
	void GuiEnd();
	void OnDestroy();
};