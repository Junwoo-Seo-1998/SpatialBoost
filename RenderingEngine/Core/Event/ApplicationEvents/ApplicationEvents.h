#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ApplicationEvents.h
Purpose: For ApplicationEvent.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include "Core/Event/Event.h"
#include <string>
#include <tuple>

class WindowResizeEvent : public Event
{
public:
	EVENT_TYPE(EventType::WindowResize);
	EVENT_CATEGORY(EventCategory::ApplicationEvent);
	WindowResizeEvent(unsigned int width, unsigned int height);
	virtual ~WindowResizeEvent() override;
	virtual std::string GetEventNameString() const override;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	std::tuple<unsigned, unsigned> GetWidthAndHeight() const;
private:
	unsigned int m_Width, m_Height;
};
