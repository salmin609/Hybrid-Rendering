#include "Level6.h"

#include <iostream>


#include "Client.h"
#include "Shader_Table.hpp"
#include "Graphic.h"
#include "LoadedObj.h"
#include "Projection.h"
#include "Object.h"
#include "PositionConverter.h"
#include "RandomNumGenerator.h"

Level6::Level6(): flockRenderVao(nullptr)
{
	flockingComputeShader = new Shader(shaderComputeFlocking.c_str());
	flockingRenderShader = new Shader(shaderFlockingVertex.c_str(), shaderFlockingFragment.c_str());

	flockBuffer = new unsigned[2];
	flockRenderVao = new unsigned[2];
	
	//objMesh = new LoadedObj("Dog", true, true);
	//std::vector<Vertex> verticesDatas = objMesh->GetVertexDatas();
	static const Vector3 geometry[] =
	{
		// Positions
		Vector3(-5.0f, 1.0f, 0.0f),
		Vector3(-1.0f, 1.5f, 0.0f),
		Vector3(-1.0f, 1.5f, 7.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 10.0f),
		Vector3(1.0f, 1.5f, 0.0f),
		Vector3(1.0f, 1.5f, 7.0f),
		Vector3(5.0f, 1.0f, 0.0f),

		// Normals
		Vector3(0.0f),
		Vector3(0.0f),
		Vector3(0.107f, -0.859f, 0.00f),
		Vector3(0.832f, 0.554f, 0.00f),
		Vector3(-0.59f, -0.395f, 0.00f),
		Vector3(-0.832f, 0.554f, 0.00f),
		Vector3(0.295f, -0.196f, 0.00f),
		Vector3(0.124f, 0.992f, 0.00f),
	};

	glGenVertexArrays(2, flockRenderVao);

	glGenBuffers(2, flockBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(flock_member), NULL, GL_DYNAMIC_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, flockBuffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, FLOCK_SIZE * sizeof(flock_member), NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &geomertyBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geomertyBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(geometry), geometry, GL_STATIC_DRAW);

	for (int i = 0; i < 2; ++i)
	{
		glBindVertexArray(flockRenderVao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, geomertyBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(8 * sizeof(Vector3)));
		
		glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[i]);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(flock_member), NULL);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(flock_member), (void*)(offsetof(flock_member, velocity)));

		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	glBindBuffer(GL_ARRAY_BUFFER, flockBuffer[0]);
	flock_member* ptr = reinterpret_cast<flock_member*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, FLOCK_SIZE * sizeof(flock_member), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	for (int i = 0; i < FLOCK_SIZE; i++)
	{
		ptr[i].position = (RandomNumber::instance->RandomVector3(-5.f, 5.f) - 0.5f) * 300.f;
		ptr[i].velocity = (RandomNumber::instance->RandomVector3(-5.f, 5.f) - 0.5f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

Level6::~Level6()
{
	glDeleteBuffers(2, flockBuffer);
	glDeleteBuffers(1, &geomertyBuffer);
	glDeleteVertexArrays(2, flockRenderVao);
}

void Level6::Load()
{
	CameraManager::instance->SetCameraPos(Vector3{ 0.f, 0.f, 300.f }, Vector3{ 0.f, 0.f, -1.f });
	Graphic::objects.clear();
	Graphic::groundId = nullptr;
}

void Level6::Update(float dt)
{
	flockingComputeShader->Use();
	(dt);
	
	int newx = 0, newy = 0;
	SDL_GetMouseState(&newx, &newy);
	const Point curr = PositionConverter::GetMousePosInWorldCoord(static_cast<float>(newx), static_cast<float>(newy));
	
	Vector3 goal{ curr.x * 2000.f, curr.y * 2000.f, 0.f };
	
	flockingComputeShader->SendUniformVec3("goal", &goal);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, flockBuffer[frameIndex]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, flockBuffer[frameIndex ^ 1]);

	glDispatchCompute(NUM_WORKGROUPS, 1, 1);
	
	static const float one = 1.0f;
	static const float black[] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	flockingRenderShader->Use();
	Matrix ndcMat = CameraToNDC(*CameraManager::instance->GetCamera());
	Affine camMat = WorldToCamera(*CameraManager::instance->GetCamera());
	
	flockingRenderShader->SendUniformMat("ndcMat", &ndcMat);
	flockingRenderShader->SendUniformMat("camMat", &camMat);

	glBindVertexArray(flockRenderVao[frameIndex]);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 8, FLOCK_SIZE);

	frameIndex ^= 1;
}

void Level6::UnLoad()
{
}
