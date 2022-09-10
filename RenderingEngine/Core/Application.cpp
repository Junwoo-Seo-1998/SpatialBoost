#include"Application.h"
Application::Application()
	:m_Window(std::make_shared<Window>())
{

}

Application::~Application()
{
}

void Application::Init()
{
	m_Window->Init();
}

void Application::Update()
{
	while (!m_Window->ShouldClose())
	{
		m_Window->Update();
	}
}

void Application::Close()
{
	m_Window->Close();
}
