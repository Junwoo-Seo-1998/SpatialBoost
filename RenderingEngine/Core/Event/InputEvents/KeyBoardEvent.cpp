/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: KeyBoardEvent.cpp
Purpose: For KeyBoardEvent.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
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