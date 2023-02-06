/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DrawingManager.cpp
Purpose: Manager for handling drawings.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#include "vmath.h"

#include "DrawingManager.h"
#include "Graphic.h"
#include "Projection.h"
#include <iostream>
#include "Ground.h"
#include "Client.h"
#include "FrameBuffer.h"
#include "Level1.h"
#include "Skybox.h"
#include "OutLine.h"
#include "Texture.h"
#include "WaterFrameBuffer.h"
#include "FrameBufferTexturing.h"
#include "WaterRenderer.h"

DrawingManager::DrawingManager()
{
	//outLine = new OutLine();
	//frameBufferObj = new FrameBufferObject(nullptr, shaderFrameBufferVertex, 
	//	shaderFrameBufferFragment);
	skyBox = new SkyBox();
	//waterRenderer = new WaterRenderer();
}

void DrawingManager::Drawing(float dt)
{
	(dt);

	object = Graphic::objects;
	if (!object.empty())
	{
		objectsSize = Graphic::objects.size();

		ndcMat = CameraToNDC(*CameraManager::instance->GetCamera());
		camMat = WorldToCamera(*CameraManager::instance->GetCamera());
		Matrix transposed = transpose(camMat);
		glBindBuffer(GL_UNIFORM_BUFFER, Graphic::instance->GetUboMatricesId());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Affine), sizeof(Affine), &transposed);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		const bool isWaterExist = Graphic::water != nullptr;

		//if (isWaterExist)
		//{
		//	glEnable(GL_CLIP_DISTANCE0);
		//	waterRenderer->GetReflectTexture()->Bind();
		//	WaterInitialize(Hcoord{ 0, 1, 0, -1.f + 0.1f });
		//	DrawingObjs();
		//	waterRenderer->GetReflectTexture()->UnBind();

		//	waterRenderer->GetRefractTexture()->Bind();
		//	WaterInitialize(Hcoord{ 0, -1, 0, 1.f });
		//	DrawingGround();
		//	DrawingObjs();
		//	waterRenderer->GetRefractTexture()->UnBind();

		//	glDisable(GL_CLIP_DISTANCE0);
		//}
		//frameBufferObj->Bind();
		//if (!isWaterExist)
		//{
		//	frameBufferObj->Bind();
		//}
		glViewport(0, 0, Client::windowWidth, Client::windowHeight);
		//ClearBuffer();
		//DrawingGround();
		//DrawingShadow();

		if(isWaterExist)
		{
			//waterRenderer->Render(dt, ndcMat, camMat);
		}
		skyBox->Draw(ndcMat);
		//outLine->OutlinePrepare();
		DrawingObjs();
		//outLine->Draw();

		//frameBufferObj->UnBind();
		//frameBufferObj->Use();
		//if (!isWaterExist)
		//{
		//	frameBufferObj->UnBind();
		//	frameBufferObj->Use();
		//}
	}
}

void DrawingManager::ClearBuffer()
{
	const Vector3& airColor = Graphic::instance->airColor;
	glClearColor(airColor.x, airColor.y, airColor.z, 1.f);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

SkyBox* DrawingManager::GetSkyBox()
{
	return skyBox;
}

void DrawingManager::DrawingGround()
{
	if (Graphic::groundId != nullptr)
	{
		Shader* groundShader = Graphic::groundId;
		static const GLfloat black[] = { 0.85f, 0.95f, 1.0f, 1.0f };
		static const GLfloat one = 1.0f;
		glViewport(0, 0, Client::windowWidth, Client::windowHeight);
		glClearBufferfv(GL_COLOR, 0, black);
		glClearBufferfv(GL_DEPTH, 0, &one);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		groundShader->Use();
		Matrix mvp = ndcMat * camMat;
		
		groundShader->SendUniformMat("mv_matrix", &camMat);
		groundShader->SendUniformMat("proj_matrix", &ndcMat);
		groundShader->SendUniformMat("mvpMat", &mvp);
		groundShader->SendUniformFloat("dmap_depth", 3.f);
		Ground::groundHeightMapTexture->Bind();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
		glDisable(GL_CULL_FACE);
	}
}
void DrawingManager::DrawingShadow()
{
	if(Graphic::light != nullptr)
	{
		Point& lightPos = Graphic::light->Get_Obj_Pos();

		glDisable(GL_DEPTH_TEST);
		Matrix shadowMat = Shadow(lightPos);
		for (size_t i = 0; i < objectsSize; ++i)
		{
			Object* obj = object[i];
			Object_ExceptShadow* shadow = obj->Get_Shadow();
			Matrix objModelMat = obj->Get_Model_To_World();

			if (shadow != nullptr)
			{
				shadow->Set_Light_Pos(lightPos);
				shadow->Draw(ndcMat, camMat, objModelMat, shadowMat);
			}
		}
		glEnable(GL_DEPTH_TEST);
	}

}

void DrawingManager::DrawingObjs()
{
	objectsSize = Graphic::objects.size();
	object = Graphic::objects;
	for (size_t i = 0; i < objectsSize; ++i)
	{
		Object* obj = object[i];
		obj->Set_Camera_Pos(CameraManager::instance->CameraPos());

		if(Graphic::light != nullptr)
			obj->Set_Light_Pos(Graphic::light->Get_Obj_Pos());
		obj->Draw();
	}
}

void DrawingManager::DrawLight()
{
	Object* lightObj = Graphic::light;

	if (lightObj != nullptr)
	{
		lightObj->Draw();
	}
}

void DrawingManager::WaterInitialize(Hcoord planeVec)
{
	if (Graphic::water != nullptr)
	{
		Object* waterObj = Graphic::water;
		const Point waterPos = waterObj->Get_Obj_Pos();
		for (size_t i = 0; i < objectsSize; ++i)
		{
			Object* obj = object[i];
			obj->SetPlane(planeVec);
		}
	}
}
