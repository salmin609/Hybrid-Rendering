#include "InputManager.h"

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
	if(instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
	instance = this;

	for(int i = 97; i < 123; ++i)
	{
		inputs.insert(std::make_pair<char, bool>(static_cast<char>(i), false));
	}
	isEmpty = true;
}

bool InputManager::IsPressed(char val)
{
	if(val < 97)
	{
		val += 32;
	}
	return inputs[val];
}

bool InputManager::IsEmpty()
{
	return isEmpty;
}

void InputManager::Press(SDL_Keycode code)
{
	const char val = static_cast<char>(code);
	inputs[val] = true;
	isEmpty = false;
}

void InputManager::Clear()
{
	for (int i = 97; i < 123; ++i)
	{
		inputs[static_cast<char>(i)] = false;
	}
	isEmpty = true;
}
