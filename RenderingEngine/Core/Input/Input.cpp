#include "Input.h"
#include <array>
#include <stack>

namespace InputStatics
{
	std::array<bool, static_cast<size_t>(KeyCode::Last)> s_pressed;/*!< Array of keys pressed this frame*/
	std::array<bool, static_cast<size_t>(KeyCode::Last)>  s_released;/*!< Array of keys released this frame*/
	std::array<bool, static_cast<size_t>(KeyCode::Last)>  s_triggered;/*!< Array of keys triggered this frame*/
	std::array<bool, static_cast<size_t>(KeyCode::Last)>  s_repeating;/*!< Array of keys triggered this frame*/

	std::stack<KeyCode> s_unpressed;            /*!< The array of keys to unpress*/
}

bool Input::IsKeyPressed(KeyCode key)
{
	return InputStatics::s_pressed[static_cast<int>(key)];
}

bool Input::IsTriggered(KeyCode key)
{
	return InputStatics::s_triggered[static_cast<int>(key)];
}

bool Input::IsRepeating(KeyCode key)
{
	return InputStatics::s_repeating[static_cast<int>(key)];
}

bool Input::IsKeyReleased(KeyCode key)
{
	return InputStatics::s_released[static_cast<int>(key)];
}

void Input::Reset()
{
	/*Only reset the pressed values that are not being pressed*/
	while (!InputStatics::s_unpressed.empty())
	{
		InputStatics::s_pressed[static_cast<int>(InputStatics::s_unpressed.top())] = false;/*Reset the pressed value*/
		InputStatics::s_unpressed.pop();
	}

	constexpr int size = static_cast<int>(InputStatics::s_pressed.size());
	for (int i = 0; i < size; ++i)
	{
		InputStatics::s_triggered[i] = false;
		InputStatics::s_repeating[i] = false;
		InputStatics::s_released[i] = false;
	}
}

void Input::SetKey(KeyCode key, bool state, bool repeating)
{
	if (state)
	{
		if (InputStatics::s_pressed[static_cast<int>(key)])
		{
			InputStatics::s_repeating[static_cast<int>(key)] = true;
		}
		else
		{
			InputStatics::s_pressed[static_cast<int>(key)] = true;
			InputStatics::s_triggered[static_cast<int>(key)] = true;
		}
	}
	else
	{
		InputStatics::s_unpressed.push(key);
		InputStatics::s_released[static_cast<int>(key)] = true;
	}
}


