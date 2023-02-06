/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Client.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/


#include <SDL2/SDL.h>
#include "Graphic.h"
class As1;
class State;
class Level1;
class Level2;
class Level3;
class Level4;
class Level5;
class Level6;
class Level7;
class Level8;
class Level9;
class Level10;
class Level11;
class DeferredRenderingState;
class InputManager;
class Client {
public:
	Client(void);
	~Client(void);

	void Set_Window_WH(int width, int height);
	void Set_Traverse_Mode(bool toggle);
	void Set_Move_Faster(bool toggle);
	void mousewheel(SDL_Event event);
	void mousepress();
	void Update_AB(Point curr_mouse_pos);
	void Move(SDL_Keycode keycode);
	void mouse_motion();
	void Set_Prev_Mousepos(Point p);
	void Set_Selected_Null();
	void draw(float dt);
	void Increase_Graphic_Level();
	void Decrease_Graphic_Level();
	static int windowWidth;
	static int windowHeight;
	//As1* as1;
	DeferredRenderingState* deState;

private:
	Graphic* graphic;
	
	/*Level1* level1;
	Level2* level2;
	Level3* level3;
	Level4* level4;
	Level5* level5;
	Level6* level6;
	Level7* level7;
	Level8* level8;
	Level9* level9;
	Level10* level10;
	Level11* level11;*/
	State* currState;
	InputManager* input;
};