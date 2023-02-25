/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs350_1
Author: Junwoo Seo, junwoo.seo, 0055213
End Header --------------------------------------------------------*/
#pragma once
#include <memory>
#include "Core/Utils/KeyCodes.h"

class Application;
class WindowGLFW;
class Input;
namespace InputStatics
{
	std::shared_ptr<Input> createInstance();
}

class Input
{
	friend Application;
	friend WindowGLFW;
public:
	Input() = delete;
	
	static bool IsPressed(Key key);
	static bool IsPressed(Mouse button);
	static bool IsTriggered(Key key);
	static bool IsRepeating(Key key);
	static bool IsReleased(Key key);
	static bool IsReleased(Mouse button);

	static std::tuple<float, float> GetMousePosition();
	static std::tuple<float, float> GetMouseOffset();
private:
	static void Reset();
	static void SetKey(Key key, bool state);
	static void SetMouseButton(Mouse button, bool state);
	static void SetMousePosition(float x, float y);

};