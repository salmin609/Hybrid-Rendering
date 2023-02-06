/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: DeferredRendering.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#pragma once
#include "Object.h"
#include "State.h"

class Camera;
class VAO;
class FrameBuffer;
class SkyBox;
class Buffer;
class GBuffer;
class DeferredRenderingState : public State
{
public:
	enum class DisplayState
	{
		NONE = 0,
		POSITION,
		NORMAL,
		UV,
		DEPTH,
	};
	DeferredRenderingState();
	~DeferredRenderingState();
	void RenderQuad();
	void RenderCube();
	void RandomizeLightPos();
	void SetMesh();
	virtual void Load();
	virtual void Update(float dt);
	virtual void UnLoad();

	void LightPass();
	DisplayState state = DisplayState::NONE;
	bool doDepthCopy = true;
	unsigned int uboLights;
	bool isPlayingNormal = false;
	bool isPlayingFaceNormal = false;
	Vector3 lightAmbient{0.f, 0.f, 0.f};
	Vector3 lightDiffuse{1.f, 1.f, 1.f};
	Vector3 lightSpec{1.f, 1.f, 1.f};
	const int lightsNum = 32;
	Mesh* objMesh;
private:
	void SetLightParameters();
	std::vector<PointLight> pointLights;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO;

	Shader* gBufferShader;
	Shader* deferredShader;
	Shader* shaderQuad;
	Shader* shaderLight;
	Shader* normalDisplayShader;
	Object* obj;
	std::vector<Object*> objManager;
	GBuffer* gBuffer;
	GBuffer* secGBuffer;
	

	Camera* cam;
};
