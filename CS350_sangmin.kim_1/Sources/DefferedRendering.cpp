/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DeferredRendering.cpp
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#include "DefferedRendering.h"

#include <iostream>

#include "Client.h"
#include "Projection.h"
#include "Texture.h"
#include "GBuffer.h"
#include "InputManager.h"
#include "RandomNumGenerator.h"
#include "readAssimpFile.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

DeferredRenderingState::DeferredRenderingState()
{
	SetMesh();

	shaderQuad = new Shader(shaderQuadVertex.c_str(), shaderQuadFragment.c_str());
	gBufferShader = new Shader(shaderGBufferVertex.c_str(), shaderGBufferFragment.c_str());
	deferredShader = new Shader(shaderDeferredShadingVertex.c_str(), shaderDeferredShadingFragment.c_str());
	shaderLight = new Shader(normalVertexShader.c_str(), normalFragmentShader.c_str());
	normalDisplayShader = new Shader(shaderNormalDisplayVertex.c_str(), shaderNormalDisplayFragment.c_str());
	gBuffer = new GBuffer();
	secGBuffer = new GBuffer();
	objMesh->SetShader(gBufferShader);

	deferredShader->Use();
	deferredShader->SendUniformInt("gPosition", 0);
	deferredShader->SendUniformInt("gNormal", 1);
	deferredShader->SendUniformInt("gAlbedoSpec", 2);

	std::vector<Color_Val> colorVec;
	Texture* texture = new Texture("../Images/metal_roof_diff_512x512.ppm", colorVec);

	colorVec.clear();
	Texture* texture2 = new Texture("../Images/metal_roof_spec_512x512.ppm", colorVec);

	objMesh->PutTexture(texture);
	objMesh->PutTexture(texture2);
	const Point initialPos{ -5.f, -5.f, 0.f };
	for (int i = 0; i < 10; ++i)
	{
		const Point newPos = initialPos + Point(static_cast<float>(i), 0.f, 0.f);
		obj = new Object(objMesh);
		obj->SetPosition(newPos);
		obj->Set_Scale(5.f);
		obj->SetNormalDisplayBuffer(normalDisplayShader);
		objManager.push_back(obj);
	}


	const size_t bufferSize = lightsNum * sizeof(PointLight);

	glGenBuffers(1, &uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboLights, 0, bufferSize);

	SetLightParameters();
	RandomizeLightPos();
}

DeferredRenderingState::~DeferredRenderingState()
{
	delete gBufferShader;
	delete deferredShader;
	delete shaderQuad;
	delete shaderLight;
	//delete obj;
	delete gBuffer;
	delete secGBuffer;


	for (int i = 0; i < objManager.size(); ++i)
	{
		delete objManager[i];
	}
}

void DeferredRenderingState::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DeferredRenderingState::RenderCube()
{
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void DeferredRenderingState::RandomizeLightPos()
{
	//constexpr unsigned int NR_LIGHTS = 32;
	for (int i = 0; i < lightsNum; i++)
	{
		float randomX = RandomNumber::instance->RandomFloat(-5.f, 5.f);
		float randomY = RandomNumber::instance->RandomFloat(-2.f, 2.f);
		float randomZ = RandomNumber::instance->RandomFloat(-2.f, 2.f);
		Vector3 randomVec{ randomX, randomY, randomZ };
		pointLights[i].position = randomVec;
	}
}

void DeferredRenderingState::SetMesh()
{
	std::vector<AssimpMeshData*> meshDatas;
	glm::vec3 v(0.f, 1.f, 0.f);
	ReadAssimpFile("../Objs/bunny.ply", glm::translate(glm::mat4(1.0f), v), meshDatas);

	objMesh = new Mesh();
	size_t size = meshDatas[0]->vertices.size();
	for (size_t i = 0; i < size; ++i)
	{
		Vertex newVertex;
		newVertex.position = meshDatas[0]->vertices[i].pnt;
		newVertex.normal = meshDatas[0]->vertices[i].nrm;
		objMesh->PutVertex(newVertex);
	}
	objMesh->calcUVs(Mesh::UVType::CYLINDRICAL_UV);

	size = meshDatas[0]->triangles.size();

	for (size_t i = 0; i < size; ++i)
	{
		objMesh->PutIndicesData(meshDatas[0]->triangles[i].x);
		objMesh->PutIndicesData(meshDatas[0]->triangles[i].y);
		objMesh->PutIndicesData(meshDatas[0]->triangles[i].z);
	}

	objMesh->CalculateDisplayingNormals();

}

void DeferredRenderingState::Load()
{
}


void DeferredRenderingState::Update(float dt)
{
	(dt);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam = CameraManager::instance->GetCamera();
	Matrix ndcMat = CameraToNDC(*cam);
	Affine camMat = WorldToCamera(*cam);

	gBufferShader->Use();
	gBufferShader->SendUniformMat("projection", &ndcMat);
	gBufferShader->SendUniformMat("view", &camMat);

	for (int i = 0; i < objManager.size(); ++i)
	{
		Matrix worldMat = objManager[i]->Get_Model_To_World();
		gBufferShader->SendUniformMat("model", &worldMat);
		objManager[i]->Draw();
	}

	if (doDepthCopy)
	{
		gBuffer->BindForReading();
		secGBuffer->BindForWriting();
		glBlitFramebuffer(0, 0, 1200, 900, 0, 0, 1200, 900, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	gBuffer->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	deferredShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer->gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer->gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer->gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBuffer->gUV);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gBuffer->gDepth);

	if (InputManager::instance->IsPressed('k'))
	{
		RandomizeLightPos();
	}

	for(int i = 0; i < lightsNum; ++i)
	{
		pointLights[i].ambient = lightAmbient;
		pointLights[i].diffuse = lightDiffuse;
		pointLights[i].specular = lightSpec;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight) * pointLights.size()
		, pointLights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	switch (state)
	{
	case DisplayState::NONE:
		deferredShader->SendUniformInt("stateType", 0);
		break;
	case DisplayState::POSITION:
		deferredShader->SendUniformInt("stateType", 1);
		break;
	case DisplayState::NORMAL:
		deferredShader->SendUniformInt("stateType", 2);
		break;
	case DisplayState::UV:
		deferredShader->SendUniformInt("stateType", 3);
		break;
	case DisplayState::DEPTH:
		deferredShader->SendUniformInt("stateType", 4);
		break;
	default:
		break;
	}

	RenderQuad();

	secGBuffer->BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 1200, 900, 0, 0, 1200, 900, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shaderLight->Use();
	shaderLight->SendUniformMat("projection", &ndcMat);
	shaderLight->SendUniformMat("view", &camMat);

	for (int i = 0; i < lightsNum; i++)
	{
		Affine modelMat = translate(Vector(pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z));
		modelMat = modelMat * scale(0.2f);
		shaderLight->SendUniformMat("model", &modelMat);
		Vector3 color = Vector3{ pointLights[i].diffuse.x, pointLights[i].diffuse.y, pointLights[i].diffuse.z };
		shaderLight->SendUniform3fv("lightColor", &color, 1);
		RenderCube();
	}
	normalDisplayShader->Use();
	Vector3 colorVal{ 1.f, 0.f, 0.f };

	if (isPlayingNormal)
	{
		for (size_t i = 0; i < objManager.size(); ++i)
		{
			if (objManager[i]->IsNormalVAOExist())
			{
				objManager[i]->BindNormalDisplayVAO();

				Matrix modelMat = objManager[i]->Get_Model_To_World();
				Matrix transformMatrix = ndcMat * camMat * modelMat;
				normalDisplayShader->SendUniformMat("vertexTransform", &transformMatrix);

				normalDisplayShader->SendUniform3fv("color", &colorVal, 1);
				const int sizeCheck = static_cast<int>(objManager[i]->Get_Mesh()->vertexNormalDisplay.size());

				glDrawArrays(GL_LINES, 0, sizeCheck);
			}
		}
	}
	if (isPlayingFaceNormal)
	{
		colorVal = Vector3{ 0.f, 0.f, 1.f };
		for (size_t i = 0; i < objManager.size(); ++i)
		{
			if (objManager[i]->IsFaceNormalVAOExist())
			{
				objManager[i]->BindFaceNormalDisplayVAO();

				Matrix modelMat = objManager[i]->Get_Model_To_World();
				Matrix transformMatrix = ndcMat * camMat * modelMat;
				normalDisplayShader->SendUniformMat("vertexTransform", &transformMatrix);

				normalDisplayShader->SendUniform3fv("color", &colorVal, 1);
				const int sizeCheck = static_cast<int>(objManager[i]->Get_Mesh()->faceNormalsDisplay.size());

				glDrawArrays(GL_LINES, 0, sizeCheck);
			}
		}
	}
	secGBuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderingState::UnLoad()
{
}

void DeferredRenderingState::LightPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gBuffer->BindForReading();

	const int width = Client::windowWidth;
	const int height = Client::windowHeight;

	int halfWidth = width / 2;
	int halfHeight = height / 2;

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, width, height, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
	glBlitFramebuffer(0, 0, width, height, 0, halfHeight, halfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, width, height, halfWidth, halfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	gBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
	glBlitFramebuffer(0, 0, width, height, halfWidth, 0, width, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);


}

void DeferredRenderingState::SetLightParameters()
{
	for (int i = 0; i < lightsNum; ++i)
	{
		Vector3 random = RandomNumber::instance->RandomVector3(-10.f, 10.f);

		Vector4 newLightPos = Vector4{ random.x, random.y, random.z, 1.f };
		PointLight newPointLight;
		newPointLight.position = newLightPos;

		pointLights.push_back(newPointLight);
	}
	Vector3 eye = CameraManager::instance->CameraPos();
	deferredShader->SendUniform3fv("viewPos", &eye, 1);
}
