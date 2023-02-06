/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.cpp
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#include "Graphic.h"
#include <algorithm>
#include "Client.h"
#include "Projection.h"
#include "PositionConverter.h"
#include <iostream>


const Point O(0, 0, 0);
const Vector EX(1, 0, 0),
EY(0, 1, 0),
EZ(0, 0, 1);
const Vector GRAY(0.5f, 0.5f, 0.5f);

Object* Graphic::light = nullptr;
Shader* Graphic::groundId = nullptr;
Object* Graphic::water = nullptr;
Graphic* Graphic::instance = nullptr;
std::vector<Object*> Graphic::objects;


Graphic::Graphic(void)
{
	instance = this;
	//drawingManager = new DrawingManager();
	cameraManager = new CameraManager();
	//raycastManager = new RaycastManager();
	
	Initialize_Camera();

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glDepthFunc(GL_LESS);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GLint bits;
	//glGetIntegerv(GL_DEPTH_BITS, &bits);
	//std::cout << "Depth buffer size : " << bits << std::endl;
	

	/*int stencilTest;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, 
		GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, 
		&stencilTest);
	std::cout << "Stencil size in bits : " << stencilTest << std::endl;
	
	GLint stencilBits = 0;
	glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
	std::cout << "Stencil size in bits : " << stencilBits << std::endl;
	
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/
}

Graphic::~Graphic(void)
{
}

void Graphic::InitUniformBlockMatrices()
{
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Affine), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(Affine));

	Matrix ndcMat = CameraToNDC(*CameraManager::instance->GetCamera());
	Matrix transposed = transpose(ndcMat);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Affine), &transposed);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Graphic::Clear()
{
	drawingManager->ClearBuffer();
}

void Graphic::Draw(float dt)
{
	drawingManager->Drawing(dt);
}

void Graphic::mousewheel(SDL_Event event)
{
	cameraManager->CameraMovement(event);
}


void Graphic::Move(SDL_Keycode keycode)
{
	cameraManager->Move(keycode);
}

void Graphic::mousepress()
{
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	const Point mouse_pos_world = PositionConverter::GetMousePosInWorldCoord(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
	obj_selected = Ray_To_Object(mouse_pos_world);
}

void Graphic::mouse_motion()
{
	raycastManager->MoveCastedObj(obj_selected);
}


void Graphic::Initialize_Camera()
{
	cameraManager->Init();
}

void Graphic::Update_AB(Point curr_mouse_pos)
{
	float speed = 0.01f;
	float newAlpha = ((prev_mouse_pos.y - curr_mouse_pos.y) * speed);
	float newBeta = ((prev_mouse_pos.x - curr_mouse_pos.x) * speed);
	cameraManager->UpdateAB(newAlpha, newBeta);
	Set_V_V();
}


void Graphic::Set_Prev_Mousepos(Point p)
{
	prev_mouse_pos = p;
}

Object* Graphic::Ray_To_Object(Point mouse_pos_world)
{
	return raycastManager->RayToObject(mouse_pos_world);
}

void Graphic::Set_Selected_Null()
{
	obj_selected = nullptr;
}

void Graphic::Set_Traverse_Mode(bool toggle)
{
	cameraManager->SetTraverseMode(toggle);
}

void Graphic::Set_V_V()
{
	cameraManager->SetVV();
}

void Graphic::Set_Move_Faster(bool toggle)
{
	cameraManager->SetMoveFaster(toggle);
}

void Graphic::Set_Window_WH(int width, int height)
{
	window_width = width;
	window_height = height;
}

Hcoord Graphic::GetWindowWH()
{
	return Hcoord(static_cast<float>(window_width), static_cast<float>(window_height), 0.f, 0.f);
}


void Graphic::Indicate_Level()
{
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << "Level		:		" << graphic_level << std::endl;
	std::cout << "------------------------------------------------\n" << std::endl;
	if (graphic_level == 0)
	{
		std::cout << "flat shading" << std::endl;
	}
	else if (graphic_level == 1)
	{
		std::cout << "smooth shading" << std::endl;
	}
	else if (graphic_level == 2)
	{
		std::cout << "cube texturing" << std::endl;
	}
	std::cout << "------------------------------------------------\n\n\n" << std::endl;
}

Point Graphic::GetPrevMousePos() const
{
	return prev_mouse_pos;
}

unsigned Graphic::GetUboMatricesId()
{
	return uboMatrices;
}

DrawingManager* Graphic::GetDrawingManager()
{
	return drawingManager;
}


Object* Graphic::SelectedObject()
{
	return obj_selected;
}
