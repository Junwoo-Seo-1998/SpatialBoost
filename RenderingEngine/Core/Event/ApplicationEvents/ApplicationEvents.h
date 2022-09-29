#pragma once
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
