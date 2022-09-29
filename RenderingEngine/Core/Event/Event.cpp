#include "Event.h"
std::string Event::GetEventNameString() const
{
	return GetEventName();
}

bool Event::IsInCategory(EventCategory category)
{
	return GetCategory() & static_cast<int>(category);
}

EventDispatcher::EventDispatcher(Event& event)
	:m_Event(event)
{}