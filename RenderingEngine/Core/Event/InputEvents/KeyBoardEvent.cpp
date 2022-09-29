#include "KeyBoardEvent.h"
#include <sstream>
KeyCode KeyBoardEvent::GetKeyCode() const
{
	return m_KeyCode;
}

int KeyBoardPressedEvent::GetRepeatCount() const
{
	return m_RepeatCount;
}

std::string KeyBoardPressedEvent::GetEventNameString() const
{
	std::stringstream ss;
	ss << GetEventName() << " : KeyCode - " << m_KeyCode << " Repeats - " << m_RepeatCount << std::endl;
	return ss.str();
}