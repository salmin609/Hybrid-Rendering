/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DrawingManager.h
Purpose: Manager for handling drawings.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#pragma once
#include <vector>
#include "Affine.h"

class Texture;
class FrameBufferObject;
class Shader;
class Object;
class SkyBox;
class OutLine;
class FrameBuffer;
class WaterFrameBuffer;
class WaterRenderer;
/**
 * \brief
 * Manager which managing the drawing methods for Graphic class
 */
class DrawingManager
{
public:
	DrawingManager();
	void Drawing(float dt);
	void ClearBuffer();
	SkyBox* GetSkyBox();
	void DrawingObjs();

private:
	
	void DrawingGround();
	void DrawingShadow();
	void DrawLight();
	void WaterInitialize(Hcoord planeVec);
	
	std::vector<Object*> object;

	Matrix camMat;
	Matrix ndcMat;
	float totalTime = 0.f;
	size_t objectsSize;
	SkyBox* skyBox;
	FrameBufferObject* frameBufferObj;
	OutLine* outLine;
	WaterRenderer* waterRenderer;
};