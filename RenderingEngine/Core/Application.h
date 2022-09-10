#pragma once
#include"Core/Window/Window.h"
class Application
{
public:
	Application();
	~Application();
	void Init();
	void Update();
	void Close();
private:
	std::shared_ptr<Window> m_Window;
};

