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
	static bool IsKeyPressed(KeyCode key);
	static bool IsTriggered(KeyCode key);
	static bool IsRepeating(KeyCode key);
	static bool IsKeyReleased(KeyCode key);
private:
	static void Reset();
	static void SetKey(KeyCode key, bool state, bool repeating);

};