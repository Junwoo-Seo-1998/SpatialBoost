#pragma once
/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: KeyBoardEvent.h
Purpose: For KeyBoardEvent.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
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