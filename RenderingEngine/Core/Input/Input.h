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
private:
	static void Reset();
	static void SetKey(Key key, bool state);
	static void SetMouseButton(Mouse button, bool state);

};