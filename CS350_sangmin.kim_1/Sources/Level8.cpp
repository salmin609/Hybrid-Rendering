#include "Level8.h"
#include "Graphic.h"
#include "Buffer.hpp"
#include "Projection.h"
#include "RandomNumGenerator.h"
#include "VAO.h"
Level8::Level8()
{
	//particleCount = 1024 * 1024 * 2;
	const int numX = 1024;
	const int numY = 1024;
	m_NumWorkGroups[0] = numX / 32;
	m_NumWorkGroups[1] = numY * 2 / 32;
	m_NumWorkGroups[2] = 1;
	particleCount = numX * numY * 2;
	const float colorIncreasingLevel = 1.0f / particleCount * 3;
	

	for (int i = 0; i < sphereCount; ++i)
	{
		const Vector3 randomVec = RandomNumber::instance->RandomVector3(-1400.f, 1400.f);
		//const float randomRadius = RandomNumber::instance->RandomFloat(100.f, 500.f);
		spheres.radii[i] = 300.f;
		spheres.centers[i].x = randomVec.x;
		spheres.centers[i].y = 0.0f;
		spheres.centers[i].z = randomVec.z;
	}

	particlePositions = new ParticlePos[particleCount];
	particleVelocities = new ParticleVelocity[particleCount];
	particleColor = new Vector3[particleCount];

	static const double cubeSize = 1400.0;
	Vector3 tempColor{0.f, 0.f, 0.f};
	
	for(int i = 0; i < particleCount; ++i)
	{
		particlePositions[i].x = static_cast<float>(-cubeSize / 2.0 + fmod((float)rand(), cubeSize));
		particlePositions[i].y = static_cast<float>(cubeSize / 3.0 + fmod((float)rand(), cubeSize));
		particlePositions[i].z = static_cast<float>(-cubeSize / 2.0 + fmod((float)rand(), cubeSize));

		particleVelocities[i].vx = -2.5f + rand() % 5 + (rand() % 10) / 10.0f;
		particleVelocities[i].vz = -2.5f + rand() % 5 + (rand() % 10) / 10.0f;
		particleVelocities[i].vy = -5.f + rand() % 10 + (rand() % 10) / 10.0f;
		particleVelocities[i].fTimeToLive = 20.f + fmod((float)rand(), 10.f);

		tempColor.x = tempColor.x < 1.f ? tempColor.x + colorIncreasingLevel : tempColor.x;
		tempColor.y = tempColor.x > 1.f ? tempColor.y + colorIncreasingLevel : tempColor.y;
		tempColor.z = tempColor.y > 1.f ? tempColor.z + colorIncreasingLevel : tempColor.z;

		particleColor[i] = tempColor;
		
		particlePositions[i].w = 1.0;
	}
	
	renderShader = new Shader(shaderParticleCollisionVertex.c_str(), 
		shaderParticleCollisionFragment.c_str(), 
		shaderParticleCollisionGeometry.c_str());

	computeShader = new Shader(shaderParticleCollisionCompute.c_str());

	for(int i = 0; i < 2; ++i)
	{
		positionBuffer[i] = new Buffer(GL_SHADER_STORAGE_BUFFER, particleCount * sizeof(ParticlePos), GL_STATIC_DRAW, particlePositions);
		velocityBuffer[i] = new Buffer(GL_SHADER_STORAGE_BUFFER, particleCount * sizeof(ParticleVelocity), GL_STATIC_DRAW, particleVelocities);
		drawVao[i] = new VAO();
	}
	colorBuffer = new Buffer(GL_ARRAY_BUFFER, particleCount * sizeof(Vector3), GL_STATIC_DRAW, particleColor);

	drawVao[0]->Bind();
	positionBuffer[0]->Bind();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	colorBuffer->Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	drawVao[1]->Bind();
	positionBuffer[1]->Bind();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	colorBuffer->Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);
}

Level8::~Level8()
{
	delete renderShader;
	delete computeShader;
	delete[] particlePositions;
	delete[] particleVelocities;

	for(int i = 0; i < 2; ++i)
	{
		delete positionBuffer[i];
		delete velocityBuffer[i];
		delete drawVao[i];
	}
	delete colorBuffer;
}

void Level8::Load()
{
	Graphic::objects.clear();
	Graphic::water = nullptr;
	Graphic::groundId = nullptr;
	CameraManager::instance->SetCameraPos(Vector3{ 0.942832f, 100.50537f, -1.57533f }, Vector3{ 0.f, -0.1f, 2.f });
}

void Level8::Update(float dt)
{
	computeShader->Use();
	computeShader->SendUniformFloat("dt", dt);
	computeShader->SendUniform3fv("sphereOffsets", &spheres.centers, sphereCount);
	computeShader->SendUniform1fv("sphereRadius", &spheres.radii, sphereCount);
	computeShader->SendUniformInt("gNumParticles", particleCount);

	positionBuffer[!index]->BindStorageBuffer(0, particleCount * sizeof(ParticlePos));
	velocityBuffer[!index]->BindStorageBuffer(1, particleCount * sizeof(ParticleVelocity));
	positionBuffer[index]->BindStorageBuffer(2, particleCount * sizeof(ParticlePos));
	velocityBuffer[index]->BindStorageBuffer(3, particleCount * sizeof(ParticleVelocity));

	glDispatchCompute(m_NumWorkGroups[0], m_NumWorkGroups[1], m_NumWorkGroups[2]);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUseProgram(0);

	glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderShader->Use();
	Matrix ndcMat = CameraToNDC(*CameraManager::instance->GetCamera());
	Matrix camMat = WorldToCamera(*CameraManager::instance->GetCamera());
	Matrix mvp = ndcMat * camMat;
	
	renderShader->SendUniformMat("viewMat", &camMat);
	renderShader->SendUniformMat("projMat", &ndcMat);
	renderShader->SendUniformMat("mvp", &mvp);

	index = !index;
	drawVao[index]->Bind();
	glDrawArrays(GL_POINTS, 0, particleCount);
	
	glBindVertexArray(0);
	glUseProgram(0);
}

void Level8::UnLoad()
{
}
