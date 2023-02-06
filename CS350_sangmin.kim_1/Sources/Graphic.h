/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Graphic.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#pragma once
#include <SDL2/SDL.h>
#include "Object.h"
#include "Object_ExceptShadow.h"
#include "DrawingManager.h"
#include "CameraManager.h"
#include "RaycastManager.h"
class Graphic
{
public:
	static Graphic* instance;
	Graphic(void);
	~Graphic(void);
	void InitUniformBlockMatrices();
	void Clear();
	void Draw(float dt);
	void mousewheel(SDL_Event event);
	void mousepress();
	void mouse_motion();
	//void ClearBuffers();
	void Initialize_Camera();
	void Update_AB(Point curr_mouse_pos);
	void Move(SDL_Keycode keycode);
	void Set_Prev_Mousepos(Point p);
	Object* Ray_To_Object(Point mouse_pos_world);
	void Set_Selected_Null();
	void Set_Traverse_Mode(bool toggle);
	void Set_V_V();
	void Set_Move_Faster(bool toggle);
	void Set_Window_WH(int width, int height);
	Hcoord GetWindowWH();
	void Indicate_Level();
	Point GetPrevMousePos() const;
	unsigned int GetUboMatricesId();
	DrawingManager* GetDrawingManager();
	
	Object* SelectedObject();
	static std::vector<Object*> objects;
	static Object* light;
	static Shader* groundId;
	static Object* water;
	float zFar = 20.0f, zNear = 0.1f;
	bool isCalcUVonGPU = false;
	Mesh::UVType texType = Mesh::UVType::CYLINDRICAL_UV;

	//l c q
	Vector3 globalAtts{ 0.09f, 1.f,0.032f };
	Vector3 airColor{0.1f, 0.1f, 0.1f};
	Vector3 ambientColor{ 0.05f, 0.05f, 0.05f };

	bool isLoadUVwithPos = true;
	bool isReflectOnly = false;
	bool isRefractOnly = false;
	float fersnelVal = 0.1f;
private:
	Point prev_mouse_pos;
	//Picked object
	Object* obj_selected = nullptr;

	int window_width;
	int window_height;

	int graphic_level = 0;
	unsigned int uboMatrices;
	/**
	 * \brief
	 * Manager 
	 */
	CameraManager* cameraManager;
	DrawingManager* drawingManager;
	RaycastManager* raycastManager;
};