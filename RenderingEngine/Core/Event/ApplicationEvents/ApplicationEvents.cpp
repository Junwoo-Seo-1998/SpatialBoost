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