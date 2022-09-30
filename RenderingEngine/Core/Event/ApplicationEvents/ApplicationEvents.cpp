/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ApplicationEvents.cpp
Purpose: For ApplicationEvent.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "ApplicationEvents.h"
#include <sstream>

WindowResizeEvent::WindowResizeEvent(unsigned width, unsigned height)
	:m_Width(width), m_Height(height)
{
}

WindowResizeEvent::~WindowResizeEvent()
{
}

std::string WindowResizeEvent::GetEventNameString() const
{
	std::stringstream ss;
	ss << GetEventName() << "-" << "Event: " << m_Width << ", " << m_Height;
	return ss.str();
}

unsigned WindowResizeEvent::GetWidth() const
{
	return m_Width;
}

unsigned WindowResizeEvent::GetHeight() const
{
	return m_Height;
}

std::tuple<unsigned, unsigned> WindowResizeEvent::GetWidthAndHeight() const
{
	return std::tuple<unsigned, unsigned>(m_Width, m_Height);
}