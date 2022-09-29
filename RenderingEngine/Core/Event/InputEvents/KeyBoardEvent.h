#pragma once
#include "Core/Event/Event.h"
#include "Core/Utils/KeyCodes.h"

class KeyBoardEvent :public Event
{
public:
	EVENT_CATEGORY(EventCategory::KeyBoardEvent);
	KeyCode GetKeyCode() const;
	KeyBoardEvent(KeyCode keycode)
		: m_KeyCode(keycode) {}
	KeyCode m_KeyCode;
};


class KeyBoardPressedEvent :public KeyBoardEvent
{
public:
	KeyBoardPressedEvent(KeyCode keycode, int repeatCount)
		:KeyBoardEvent(keycode), m_RepeatCount(repeatCount) {}
	EVENT_TYPE(EventType::KeyPressed);
	int GetRepeatCount() const;
	virtual std::string GetEventNameString() const override;;
private:
	int m_RepeatCount;
};