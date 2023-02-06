#pragma once
#include <SDL2/SDL_keycode.h>
#include <map>
class InputManager
{
public:
	InputManager();
	static InputManager* instance;
	bool IsPressed(char val);
	bool IsEmpty();
	void Press(SDL_Keycode code);
	void Clear();
private:
	std::map<char, bool> inputs;
	bool isEmpty;
};
