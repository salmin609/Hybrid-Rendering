#include "Level10.h"

#include <iostream>
#include <GL/glew.h>
#include "Buffer.hpp"
#include "Camera.h"
#include "CameraManager.h"
#include "Client.h"
#include "FluidCompute.h"
#include "InputManager.h"
#include "Projection.h"
#include "RandomNumGenerator.h"
#include "Shader.h"
#include "Shader_Table.hpp"
#include "TgaTexture.h"
Level10::Level10()
{
	fluid = new FluidCompute();
	//render = new Shader(shaderFluidVertex.c_str(), shaderFluidFragment.c_str(), shaderFluidGeometry.c_str());
	render = new Shader(shaderFluidVertex.c_str(), shaderFluidFragment.c_str());
	compute = new Shader(shaderFluidCompute.c_str());
	computeNeighbor = new Shader(shaderFluidComputeNeighbor.c_str());
	boxRender = new Shader(shader_simple_vertex.c_str(), shader_simple_fragment.c_str());
	pxNum = fluid->PxNum();
	pyNum = fluid->PyNum();
	pzNum = fluid->PzNum();
	pTotalNum = pxNum * pyNum * pzNum;
	ogTotalNum = pTotalNum;

	WaterParticle* particles = fluid->Particles();

	std::vector<Vector3> particlePosVec = fluid->ParticlePos();
	Neighbors neighbor;
	for(int i = 0; i < neighborCount; ++i)
	{
		neighbor.neighbor[i] = -1;
	}
	for(int i = 0; i < pTotalNum; ++i)
	{
		colors.push_back(Vector3(0.f, 0.3764705882f, 1.f));
		//radii.push_back(pointSize);
		Vector3 posVal = particles[i].pos;
		Vector3 velVal = particles[i].vel;
		Vector3 forceVal = particles[i].force;
		Vector3 particlePosVal = particlePosVec[i];
		int idVal = particles[i].id;
		float densityVal = particles[i].density;
		float lambdaVal = particles[i].lambda;

		ParticleVec4 vec4Val;
		vec4Val.velocity = Vector4{ velVal.x, velVal.y, velVal.z, 1.f };
		vec4Val.predictedPos = Vector4{ particlePosVal.x, particlePosVal.y, particlePosVal.z, 1.f };

		ParticleVal particleValVal;
		particleValVal.id = idVal;
		particleValVal.lambda = lambdaVal;
		particleValVal.density = densityVal;

		BubbleVec4 bubbleVec4Val;
		bubbleVec4Val.bubblePos = Vector4{ 0.f, 0.f, 0.f, 1.f };
		bubbleVec4Val.bubbleVel  = Vector4{ 0.f, 0.f, 0.f, 0.f };

		particleVec4Vals.push_back(vec4Val);
		particleVals.push_back(particleValVal);
		
		pos.emplace_back(posVal.x, posVal.y, posVal.z, 1.f);
		vel.emplace_back(velVal.x, velVal.y, velVal.z, 1.f);
		force.emplace_back(forceVal.x, forceVal.y, forceVal.z, 1.f);
		ids.push_back(idVal);
		density.push_back(densityVal);
		lambda.push_back(lambdaVal);
		predictedPos.emplace_back(particlePosVec[i].x, particlePosVec[i].y, particlePosVec[i].z, 1.f);
		neighbors.push_back(neighbor);
		neighborsCheckCount.push_back(0);

		bubbleVec4Vals.push_back(bubbleVec4Val);

	}
	
	compute->Use();


	
	particleValVec4Buffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(ParticleVec4) * pTotalNum, GL_DYNAMIC_DRAW, particleVec4Vals.data());
	particleValVec4Buffer->BindStorage(0);

	particleValBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(ParticleVal) * pTotalNum, GL_DYNAMIC_DRAW, particleVals.data());
	particleValBuffer->BindStorage(1);

	particleNeighbors = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Neighbors) * pTotalNum, GL_DYNAMIC_DRAW, neighbors.data());
	particleNeighbors->BindStorage(2);

	particleForceBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4) * pTotalNum, GL_DYNAMIC_DRAW, force.data());
	particleForceBuffer->BindStorage(3);

	particlePos = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4) * pTotalNum, GL_DYNAMIC_DRAW, pos.data());
	particlePos->BindStorage(4);

	particleNeighborsCheckCount = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * pTotalNum, GL_DYNAMIC_DRAW, neighborsCheckCount.data());
	
	computeNeighbor->Use();
	particlePos->BindStorage(0);
	particleNeighbors->BindStorage(1);
	particleNeighborsCheckCount->BindStorage(2);
	
	vertexBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, nullptr);
	colorBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, colors.data());
	//radiiBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(float) * pTotalNum, GL_STATIC_DRAW, radii.data());
	colorBufferBubble = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, colors.data());
	//radiiBufferBubble = new Buffer(GL_ARRAY_BUFFER, sizeof(float) * pTotalNum, GL_STATIC_DRAW, radii.data());

	tgaTexture = new TgaTexture("Particle.tga");
	tgaTexture->Use(render->GetShaderId());

	boxPositionBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector4) * 36, GL_STATIC_DRAW, nullptr);
}

Level10::~Level10()
{
	delete fluid;
	delete vertexBuffer;
	delete colorBuffer;
	//delete radiiBuffer;

	delete render;
	delete compute;
	delete computeNeighbor;

	delete particleValVec4Buffer;
	delete particleValBuffer;
	/*delete particlePos;
	delete particleForce;
	delete particleVel;
	delete particleId;
	delete particleDensity;
	delete particleLambda;
	delete particlePredictedPos;
	delete particleNeighbors;*/
}

void Level10::Load()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	CameraManager::instance->SetCameraPos(Vector3{ 0.326587f, -0.116406f, 2.73291f }, Vector3{ 0.f, -1.0f, -1.f });
	glPointSize(10.f);
}


void Level10::Update(float dt)
{
	if(InputManager::instance->IsPressed('q'))
	{
		std::vector<ParticleVec4> prevVec4Val = particleValVec4Buffer->Check<ParticleVec4>();
		std::vector<ParticleVal> prevValVal = particleValBuffer->Check<ParticleVal>();
		std::vector<Neighbors> prevNeighborVal = particleNeighbors->Check<Neighbors>();
		std::vector<Vector4> prevPosVal = particlePos->Check<Vector4>();
		std::vector<int> prevNeighborCheckVal = particleNeighborsCheckCount->Check<int>();
		std::vector<Vector3> prevColorVal = colorBuffer->Check<Vector3>();
		std::vector<Vector4> prevForceVal = particleForceBuffer->Check<Vector4>();
		//std::vector<Vector3> prevRadiiVal = radiiBuffer->Check<Vector3>();
		
		UnLoad();

		pTotalNum += ogTotalNum;

		

		prevVec4Val.insert(prevVec4Val.end(), particleVec4Vals.begin(), particleVec4Vals.end());
		prevValVal.insert(prevValVal.end(), particleVals.begin(), particleVals.end());
		prevNeighborVal.insert(prevNeighborVal.end(), neighbors.begin(), neighbors.end());
		prevPosVal.insert(prevPosVal.end(), pos.begin(), pos.end());
		prevNeighborCheckVal.insert(prevNeighborCheckVal.end(), neighborsCheckCount.begin(), neighborsCheckCount.end());
		prevForceVal.insert(prevForceVal.end(), force.begin(), force.end());
		
		prevColorVal.insert(prevColorVal.end(), colors.begin(), colors.end());
		//prevRadiiVal.insert(prevRadiiVal.end(), radii.begin(), radii.end());
		
		particleValVec4Buffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(ParticleVec4) * pTotalNum, GL_DYNAMIC_DRAW, prevVec4Val.data());
		particleValBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(ParticleVal) * pTotalNum, GL_DYNAMIC_DRAW, prevValVal.data());
		particleNeighbors = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Neighbors) * pTotalNum, GL_DYNAMIC_DRAW, prevNeighborVal.data());
		//bubbleType = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * pTotalNum, GL_DYNAMIC_DRAW, bubbleTypes.data());
		//bubbleRadius = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * pTotalNum, GL_DYNAMIC_DRAW, bubbleRadiuses.data());
		//bubbleVec4 = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(BubbleVec4) * pTotalNum, GL_DYNAMIC_DRAW, bubbleVec4Vals.data());
		//bubbleLifetime = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(float) * pTotalNum, GL_DYNAMIC_DRAW, bubbleLifetimes.data());
		particlePos = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4) * pTotalNum, GL_DYNAMIC_DRAW, prevPosVal.data());
		particleNeighborsCheckCount = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(int) * pTotalNum, GL_DYNAMIC_DRAW, prevNeighborCheckVal.data());
		particleForceBuffer = new Buffer(GL_SHADER_STORAGE_BUFFER, sizeof(Vector4) * pTotalNum, GL_DYNAMIC_DRAW, prevForceVal.data());

		
		vertexBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, nullptr);
		colorBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, prevColorVal.data());
		//radiiBuffer = new Buffer(GL_ARRAY_BUFFER, sizeof(float) * pTotalNum, GL_STATIC_DRAW, prevRadiiVal.data());
		//colorBufferBubble = new Buffer(GL_ARRAY_BUFFER, sizeof(Vector3) * pTotalNum, GL_STATIC_DRAW, colors.data());
		//radiiBufferBubble = new Buffer(GL_ARRAY_BUFFER, sizeof(float) * pTotalNum, GL_STATIC_DRAW, radii.data());
	}
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	
	fpsTimer += dt;
	gfsCount++;

	if (gfsCount > 10000)
	{
		gfsCount = 0;
		fpsTimer = 0.f;
	}
	//fluid->Update();
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	computeNeighbor->Use();
	computeNeighbor->SendUniformInt("particleNum", pTotalNum);
	
	particlePos->BindStorage(0);
	particleNeighbors->BindStorage(1);
	particleNeighborsCheckCount->BindStorage(2);

	glDispatchCompute((pTotalNum / 128) + 1, 1, 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	compute->Use();

	particleValVec4Buffer->BindStorage(0);
	particleValBuffer->BindStorage(1);
	particleNeighbors->BindStorage(2);
	particleForceBuffer->BindStorage(3);
	particlePos->BindStorage(4);

	if(InputManager::instance->IsPressed('z'))
	{
		d -= 0.1f;
		wxMin = -d;
		wxMax = 2.f * d;
		wyMin = -2.f * d;
		wyMax = d;
		wzMin = -d;
		wzMax = d;
		
	}
	if (InputManager::instance->IsPressed('x'))
	{
		d += 0.1f;
		wxMin = -d;
		wxMax = 2.f * d;
		wyMin = -2.f * d;
		wyMax = d;
		wzMin = -d;
		wzMax = d;
	}
	if (InputManager::instance->IsPressed('c'))
	{
		wxMax += 3.f;
	}
	if (InputManager::instance->IsPressed('v'))
	{
		wyMax += 3.f;
	}
	if (InputManager::instance->IsPressed('r'))
	{
		wxMax += 0.1f;
	}
	if (InputManager::instance->IsPressed('e'))
	{
		wxMax -= 0.1f;
	}
	if (InputManager::instance->IsPressed('f'))
	{
		std::vector<Vector4> prevForceVal = particleForceBuffer->Check<Vector4>();

		size_t size = prevForceVal.size();
		for(size_t i = 0; i < size; ++i)
		{
			prevForceVal[i] += Vector4(0.f, 0.f, 1000.f, 0.f);
		}
		particleForceBuffer->WriteData<Vector4>(prevForceVal);
	}

	compute->SendUniformFloat("d", d);
	compute->SendUniformFloat("wxMin", wxMin);
	compute->SendUniformFloat("wxMax", wxMax);
	compute->SendUniformFloat("wyMin", wyMin);
	compute->SendUniformFloat("wyMax", wyMax);
	compute->SendUniformFloat("wzMin", wzMin);
	compute->SendUniformFloat("wzMax", wzMax);
	compute->SendUniformInt("pTotalCount", pTotalNum);

	glDispatchCompute((pTotalNum / 128) + 1, 1 , 1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	const Camera cam = *CameraManager::instance->GetCamera();
	Matrix ndcMat = CameraToNDC(cam);
	Affine camMat = WorldToCamera(cam);
	Matrix mvp = ndcMat * camMat;
	Point camEye = cam.Eye();
	Vector4 camEyeVec4 = Vector4{ camEye.x, camEye.y, camEye.z, 1.f };
	static Vector3  lightDir = Vector3{ 1.f, 0.f, 0.f };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);

	//std::cout << "x : " << camEye.x << " " << "y : " << camEye.y << "z : " << camEye.z << std::endl;
	
	render->Use();

	render->SendUniformMat("MVP", &mvp);
	render->SendUniformMat("MV", &camMat);
	render->SendUniform3fv("lightdir", &lightDir, 1);
	render->SendUniformFloat("radius", pointSize);

	particlePos->Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)0);
	glEnableVertexAttribArray(0);

	//colorBuffer->Check<Vector3>();
	
	colorBuffer->Bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_POINTS, 0, pTotalNum);
	glDisableVertexAttribArray(0);
	
	particleNeighborsCheckCount->BindStorage();
	int* neighborCountCheck = reinterpret_cast<int*>(glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, pTotalNum * sizeof(int),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	for (int i = 0; i < pTotalNum; ++i)
	{
		neighborCountCheck[i] = 0;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//bubbleTypeCheck.reserve(pTotalNum);
}

void Level10::UnLoad()
{
	delete particleValVec4Buffer;
	delete particleValBuffer;
	delete particleNeighbors;
	//delete bubbleType;
	//delete bubbleRadius;
	//delete bubbleVec4;
	//delete bubbleLifetime;
	delete particlePos;
	delete vertexBuffer;
	delete colorBuffer;
	delete particleForceBuffer;
	//delete radiiBuffer;
	//delete colorBufferBubble;
	//delete radiiBufferBubble;
	delete particleNeighborsCheckCount;
}

