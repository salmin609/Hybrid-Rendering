/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "Client.h"
#include "InputManager.h"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

//#include "As1.h"
#include "DefferedRendering.h"

int main(int /*argc*/, char* /*argv*/[]) {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	const char* title = "Sangmin.Kim Graphic Project";
	int width = Client::windowWidth,
		height = Client::windowHeight;
	SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, width, height,
		SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	bool traverse_mode = false;
	bool move_faster_mode = false;

	// GLEW: get function bindings (if possible)
	GLenum value = glewInit();
	if (value != GLEW_OK) {
		std::cout << glewGetErrorString(value) << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_Quit();
		return -1;
	}

	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init();
	//std::memset(inputTextBuf, 0, 100);
	//bool check = false;
	//bool check2 = false;
	

	// animation loop
	try {
		Client* client = new Client();
		client->Set_Window_WH(width, height);
		Uint32 ticks_last = SDL_GetTicks();
		bool done = false;
		
		
		//const char* currentObjFile = "bunny";

		//float* planePos = reinterpret_cast<float*>(&client->as1->plane->Get_Obj_Pos());

		while (!done) {

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();

			ImGui::Begin("CS350 Sangmin Kim");                          // Create a window called "Hello, world!" and append into it.

			if(ImGui::Button("Randomize Light Pos"))
			{
				client->deState->RandomizeLightPos();
			}
			if(ImGui::Button("Toggle Position Target"))
			{
				client->deState->state = DeferredRenderingState::DisplayState::POSITION;
			}
			if (ImGui::Button("Toggle Normal Target"))
			{
				client->deState->state = DeferredRenderingState::DisplayState::NORMAL;
			}
			if (ImGui::Button("Toggle UV Target"))
			{
				client->deState->state = DeferredRenderingState::DisplayState::UV;
			}
			if (ImGui::Button("Toggle Depth Target"))
			{
				client->deState->state = DeferredRenderingState::DisplayState::DEPTH;
			}
			if (ImGui::Button("Default Rendering"))
			{
				client->deState->state = DeferredRenderingState::DisplayState::NONE;
			}
			if (ImGui::Button("Toggle DepthCopy"))
			{
				client->deState->doDepthCopy = !client->deState->doDepthCopy;
			}
			if (ImGui::Button("Visualize Normal Lines"))
			{
				client->deState->isPlayingNormal = !client->deState->isPlayingNormal;
			}
			if (ImGui::Button("Visualize Face Normal Lines"))
			{
				client->deState->isPlayingFaceNormal = !client->deState->isPlayingFaceNormal;
			}

			ImGui::SliderFloat3("lightDiffuse", reinterpret_cast<float*>(&client->deState->lightDiffuse), 0.f, 1.f);
			ImGui::SliderFloat3("lightAmbient", reinterpret_cast<float*>(&client->deState->lightAmbient), 0.f, 1.f);
			ImGui::SliderFloat3("lightSpec", reinterpret_cast<float*>(&client->deState->lightSpec), 0.f, 1.f);

			ImGui::End();

			SDL_Event event;
			while (SDL_PollEvent(&event)) 
			{
				//ImGui_ImplSDL2_ProcessEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
				{
					const SDL_Keycode code = event.key.keysym.sym;

					InputManager::instance->Press(code);
						
					if (code == SDLK_ESCAPE)
						done = true;
					else if (code == SDLK_SPACE)
					{
						traverse_mode = !traverse_mode;
						client->Set_Traverse_Mode(traverse_mode);
					}
					else if (code == SDLK_LSHIFT)
					{
						move_faster_mode = !move_faster_mode;
						client->Set_Move_Faster(move_faster_mode);
					}
					else if (code == SDLK_w ||
						code == SDLK_a ||
						code == SDLK_s ||
						code == SDLK_d)
					{
						client->Move(code);
					}
					else if (code == SDLK_RIGHT)
					{
						//client->Increase_Graphic_Level();
					}
					else if (code == SDLK_LEFT)
					{
						//client->Decrease_Graphic_Level();
					}
				}
					break;
				case SDL_MOUSEWHEEL:
					client->mousewheel(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
					client->mousepress();
					break;
				case SDL_MOUSEMOTION:
					int mouse_x, mouse_y;

					SDL_GetMouseState(&mouse_x, &mouse_y);

					if (traverse_mode == true)
					{
						client->Update_AB(Point(static_cast<float>(mouse_x), static_cast<float>(mouse_y), 0));
					}
					client->mouse_motion();
					client->Set_Prev_Mousepos(Point(static_cast<float>(mouse_x), static_cast<float>(mouse_y), 0));
					break;
				case SDL_MOUSEBUTTONUP:
					client->Set_Selected_Null();
					break;
				}
			}
			Uint32 ticks = SDL_GetTicks();
			float dt = 0.001f * (ticks - ticks_last);
			ticks_last = ticks;
			client->draw(dt);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			SDL_GL_SwapWindow(window);
		}

		delete client;
	}

	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

