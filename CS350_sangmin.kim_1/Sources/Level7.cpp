#include "Level7.h"
#include "Light_Mesh.h"
#include "Graphic.h"
#include "Projection.h"
#include "Shader.h"
#include "Shader_Table.hpp"
#include "Texture.h"
#include "vs2017/SimpleMeshes.h"
#include "VAO.h"
#include <iostream>
#include "Client.h"
#include "FrameBuffer.h"
#include "Buffer.hpp"
#include "InputManager.h"
#include "Shader_Table.hpp"
/*
 * renderingShader = new Shader(shaderFrameBufferVertex.c_str(), shaderFrameBufferFragment.c_str());
	renderingShader->Use();
	renderingShader->SendUniformInt("screenTextures", 0);

	computeShader = new Shader(shaderComputeRaytracing.c_str());
	computeShader->Use();

	texture = new Texture(GL_RGBA32F);
	texture->BindImageTexture(0, GL_WRITE_ONLY);

	frameBufferObj = new FrameBufferObject(texture, shaderFrameBufferVertex, shaderFrameBufferFragment);


	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	computeShader->Use();
	const Point camPos = CameraManager::instance->GetCamera()->Eye();
	Vector3 pos{ camPos.x, camPos.y, camPos.z };

	Matrix camMat = WorldToCamera(*CameraManager::instance->GetCamera());
	Matrix ndcMat = CameraToNDC(*CameraManager::instance->GetCamera());
	Matrix inverseViewProjectMatrix = inverse(ndcMat * camMat);

	Hcoord ray00val = inverseViewProjectMatrix * Hcoord(-1.f, -1.f, 0.f, 1.f);
	ray00val /= ray00val.w;
	ray00val = ray00val - camPos;

	Hcoord ray01val = inverseViewProjectMatrix * Hcoord(-1.f, 1.f, 0.f, 1.f);
	ray01val /= ray01val.w;
	ray01val = ray01val - camPos;

	Hcoord ray10val = inverseViewProjectMatrix * Hcoord(1.f, -1.f, 0.f, 1.f);
	ray10val /= ray10val.w;
	ray10val = ray10val - camPos;

	Hcoord ray11val = inverseViewProjectMatrix * Hcoord(1.f, 1.f, 0.f, 1.f);
	ray11val /= ray11val.w;
	ray11val = ray11val - camPos;

	Vector3 ray00 = Vector3{ ray00val.x, ray00val.y, ray00val.z };
	Vector3 ray01 = Vector3{ ray01val.x, ray01val.y, ray01val.z };
	Vector3 ray10 = Vector3{ ray10val.x, ray10val.y, ray10val.z };
	Vector3 ray11 = Vector3{ ray11val.x, ray11val.y, ray11val.z };

	computeShader->SendUniformVec3("eye", &pos);
	computeShader->SendUniformVec3("ray00", &ray00);
	computeShader->SendUniformVec3("ray01", &ray01);
	computeShader->SendUniformVec3("ray10", &ray10);
	computeShader->SendUniformVec3("ray11", &ray11);

	glDispatchCompute(64, 100, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	frameBufferObj->Bind();
	frameBufferObj->UnBind();
	frameBufferObj->Use();
 */

Level7::Level7()
{
	max_depth = 1;
	debugMode = DEBUG_NONE;
	InitShader();
	InitBuffer();
}

Level7::~Level7()
{
	
}

void Level7::Load()
{
	Graphic::objects.clear();
	Graphic::water = nullptr;
	Graphic::groundId = nullptr;
	CameraManager::instance->SetCameraPos(Vector3{ 0.942832f, 1.50537f, -1.57533f }, Vector3{ 0.f, -0.1f, 2.f });
}

void Level7::Update(float dt)
{
	(dt);
	if(!InputManager::instance->IsEmpty())
	{
		if(InputManager::instance->IsPressed('e'))
		{
			max_depth++;
		}
		if (InputManager::instance->IsPressed('q'))
		{
			max_depth--;
		}
	}
	static const Vector4 zero;
	static const Vector4 gray = Vector4(0.1f, 0.1f, 0.1f, 0.0f);
	static const Vector4 one(1.f, 1.f, 1.f, 1.f);
	Camera cam = *CameraManager::instance->GetCamera();
	Matrix ndcMat = CameraToNDC(cam);
	Matrix ndcMatTransposed = transpose(ndcMat);
	Affine camMat = WorldToCamera(cam);
	Matrix camMatTransposed = transpose(camMat);
	Vector3 viewPos = cam.Eye();

	uniformBuffer->Bind(0);
	uniforms_block* block = (uniforms_block*)glMapBufferRange(GL_UNIFORM_BUFFER,
		0,
		sizeof(uniforms_block),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	//Matrix modelMat = scale(7.f);
	//modelMat = transpose(modelMat);
	block->mv_matrix = camMatTransposed;
	block->view_matrix = camMatTransposed;
	block->proj_matrix = ndcMatTransposed;

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	sphereBuffer->Bind(1);
	sphere* s = (sphere*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(sphere),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for(int i = 0 ; i < 128; ++i)
	{
		float fi = (float)i / 128.0f;
		s[i].center = Vector3(sinf(fi * 123.0f) * 15.75f, cosf(fi * 456.0f) * 15.75f, (sinf(fi * 300.0f) * cosf(fi * 200.0f)) * 20.0f);
		s[i].radius = fi * 2.3f + 3.5f;
		float r = fi * 61.f;
		float g = r + 0.25f;
		float b = g + 0.25f;

		r = (r - floorf(r)) * 0.8f + 0.2f + dt;
		g = (g - floorf(g)) * 0.8f + 0.2f + dt;
		b = (b - floorf(b)) * 0.8f + 0.2f + dt;
		s[i].color = Vector4(r, g, b, 1.f);
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	planeBuffer->Bind(2);
	plane* p = (plane*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(plane),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	p[0].normal = Vector3(0.0f, 0.0f, -1.f);
	p[0].d = 30.f;

	p[1].normal = Vector3(0.0f, 0.0f, 1.f);
	p[1].d = 30.f;

	p[2].normal = Vector3(-1.0f, 0.0f, 0.f);
	p[2].d = 30.f;
	
	p[3].normal = Vector3(1.0f, 0.0f, 0.f);
	p[3].d = 30.f;

	p[4].normal = Vector3(0.0f, -1.0f, 0.f);
	p[4].d = 30.f;

	p[5].normal = Vector3(0.0f, 1.0f, 0.f);
	p[5].d = 30.f;

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	lightBuffer->Bind(3);
	light* l = (light*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 128 * sizeof(light),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for(int i = 0 ; i < 128; ++i)
	{
		float fi = 3.33f - (float)i;
		l[i].position = Vector3(sinf(fi * 2.0f) * 15.75f,
			cosf(fi * 5.0f) * 5.75f,
			(sinf(fi * 3.0f) * cosf(fi * 2.5f)) * 19.4f);
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	vao->Bind();
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	float aspectVal = (float)Client::windowHeight / (float)Client::windowWidth;
	prepareProgram->Use();
	prepareProgram->SendUniformMat("rayLookat", &camMat);
	prepareProgram->SendUniform3fv("rayOrigin", &viewPos, 1);
	prepareProgram->SendUniform1fv("aspect", &aspectVal, 1);
	
	glBindFramebuffer(GL_FRAMEBUFFER, rayFbo[0]);

	static const GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5
	};

	glDrawBuffers(6, drawBuffers);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	traceProgram->Use();
	Recurse(0);

	blitProgram->Use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

	glActiveTexture(GL_TEXTURE0);
	switch(debugMode)
	{
	case DEBUG_NONE:
		glBindTexture(GL_TEXTURE_2D, texComposite);
		break;
	case DEBUG_REFLECTED:
		//glBindTexture(GL_TEXTURE_2D, texReflected[de]);
		break;
	case DEBUG_REFRACTED: break;
	case DEBUG_REFLECTED_COLOR: break;
	case DEBUG_REFRACTED_COLOR: break;
	default: 
		break;
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Level7::UnLoad()
{
}

void Level7::Recurse(int depth)
{
	glBindFramebuffer(GL_FRAMEBUFFER, rayFbo[depth + 1]);

	static const GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5
	};

	glDrawBuffers(6, drawBuffers);

	glEnablei(GL_BLEND, 0);
	glBlendFunci(0, GL_ONE, GL_ONE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texPosition[depth]);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texReflected[depth]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texReflectionIntensity[depth]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if(depth != (max_depth - 1))
	{
		Recurse(depth + 1);
	}

	glDisablei(GL_BLEND, 0);
}

void Level7::InitShader()
{
	prepareProgram = new Shader(shaderRaytracingPrepareVertex.c_str(), shaderRaytracingPrepareFragment.c_str());
	traceProgram = new Shader(shaderRaytracingVertex.c_str(), shaderRaytracingFragment.c_str());
	blitProgram = new Shader(shaderRaytracingBlitVertex.c_str(), shaderRaytracingBlitFragment.c_str());
}

void Level7::InitBuffer()
{
	uniformBuffer = new Buffer(GL_UNIFORM_BUFFER, sizeof(uniforms_block), GL_DYNAMIC_DRAW, nullptr);
	sphereBuffer = new Buffer(GL_UNIFORM_BUFFER, 128 * sizeof(sphere), GL_DYNAMIC_DRAW, nullptr);
	planeBuffer = new Buffer(GL_UNIFORM_BUFFER, 128 * sizeof(plane), GL_DYNAMIC_DRAW, nullptr);
	lightBuffer = new Buffer(GL_UNIFORM_BUFFER, 128 * sizeof(sphere), GL_DYNAMIC_DRAW, nullptr);

	vao = new VAO();
	glGenFramebuffers(MAX_RECURSION_DEPTH, rayFbo);
	glGenTextures(1, &texComposite);
	glGenTextures(MAX_RECURSION_DEPTH, texPosition);
	glGenTextures(MAX_RECURSION_DEPTH, texReflected);
	glGenTextures(MAX_RECURSION_DEPTH, texRefracted);
	glGenTextures(MAX_RECURSION_DEPTH, texReflectionIntensity);
	glGenTextures(MAX_RECURSION_DEPTH, texRefractionIntensity);

	glBindTexture(GL_TEXTURE_2D, texComposite);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);

	for(int i = 0; i < MAX_RECURSION_DEPTH; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rayFbo[i]);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texComposite, 0);

		glBindTexture(GL_TEXTURE_2D, texPosition[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, texPosition[i], 0);

		glBindTexture(GL_TEXTURE_2D, texReflected[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, texReflected[i], 0);

		glBindTexture(GL_TEXTURE_2D, texRefracted[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, texRefracted[i], 0);

		glBindTexture(GL_TEXTURE_2D, texReflectionIntensity[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, texReflectionIntensity[i], 0);

		glBindTexture(GL_TEXTURE_2D, texRefractionIntensity[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, MAX_FB_WIDTH, MAX_FB_HEIGHT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, texRefractionIntensity[i], 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
