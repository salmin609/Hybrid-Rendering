//#include "Level11.h"
//#include <iostream>
//#include <GL/glew.h>
//#include "Shader_Table.hpp"
//#include "Shader.h"
//#include "vs2017/Test.cuh"
//#include "Buffer.hpp"
//#include "Client.h"
//#include "Graphic.h"
//#include "Projection.h"
//#include "PositionConverter.h"
//#include "RandomNumGenerator.h"
//#include "InputManager.h"
//#include <stdlib.h>
//#include "BinaryFileManager.h"
//#include "CudaBuffer.hpp"
//Level11::Level11()
//{
//	render = new Shader(shader2DVertex.c_str(), shader2DFragment.c_str());
//	renderOnePoint = new Shader(shaderOnePointVertex.c_str(), shader2DFragment.c_str());
//
//	sandParticle = new CudaBuffer<ParticleSand>(particleMemSize);
//	sandGrid = new CudaBuffer<ParticleGrid>(gridMemSize);
//	land = new CudaBuffer<Land>(landMemSize);
//	spawnerPos = new CudaBuffer<SpawnerPos>(spawnerMemSize);
//	waterParticle = new CudaBuffer<ParticleWater>(waterMemSize);
//	firstContinuouslySpawn = new CudaBuffer<SpawnerPos>(contiSpawnerMemSize);
//	secondContinuouslySpawn = new CudaBuffer<SpawnerPos>(contiSpawnerMemSize);
//	
//	/*CopyFromHostToDevice();
//	Init(NUMPARTICLES, NUMGRIDS, NUMSPAWNERS, NUMCONTISPAWNERS, sandParticle->GetDeviceMemoryPtr(), 
//		sandGrid->GetDeviceMemoryPtr(), land->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr(),
//		firstContinuouslySpawn->GetDeviceMemoryPtr(), secondContinuouslySpawn->GetDeviceMemoryPtr());
//	CopyFromDeviceToHost();
//	LoadMap();
//
//	sandPosBuffer = new Buffer(GL_ARRAY_BUFFER, particleMemSize, GL_STATIC_DRAW, sandParticle->GetHostMemoryPtr());
//	gridPosBuffer = new Buffer(GL_ARRAY_BUFFER, gridMemSize, GL_STATIC_DRAW, sandGrid->GetHostMemoryPtr());
//	landPosBuffer = new Buffer(GL_ARRAY_BUFFER, landMemSize, GL_STATIC_DRAW, land->GetHostMemoryPtr());
//	spawnerPosBuffer = new Buffer(GL_ARRAY_BUFFER, spawnerMemSize, GL_STATIC_DRAW, spawnerPos->GetHostMemoryPtr());
//	waterPosBuffer = new Buffer(GL_ARRAY_BUFFER, waterMemSize, GL_STATIC_DRAW, waterParticle->GetHostMemoryPtr());
//	firstContiPosBuffer = new Buffer(GL_ARRAY_BUFFER, contiSpawnerMemSize, GL_STATIC_DRAW, firstContinuouslySpawn->GetHostMemoryPtr());
//	secondContiPosBuffer = new Buffer(GL_ARRAY_BUFFER, contiSpawnerMemSize, GL_STATIC_DRAW, secondContinuouslySpawn->GetHostMemoryPtr());*/
//
//	firstContiPosBuffer->Check<Vector2>();
//	secondContiPosBuffer->Check<Vector2>();
//}
//
//Level11::~Level11()
//{
//	delete render;
//	delete sandPosBuffer;
//	delete gridPosBuffer;
//	delete landPosBuffer;
//	delete sandGrid;
//	delete sandParticle;
//	delete land;
//	delete waterParticle;
//	delete waterPosBuffer;
//	delete firstContinuouslySpawn;
//	delete secondContinuouslySpawn;
//}
//
//void Level11::Load()
//{
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glClearColor(1.f, 1.f, 1.f, 1.f);
//	Graphic::objects.clear();
//	CameraManager::instance->SetCameraPos(Vector3{ 0.f, 0.f, -1.f }, Vector3{ 0.f, 0.f, 1.f });
//}
//
//void Level11::Update(float dt)
//{
//	(dt);
//
//	const Camera cam = *CameraManager::instance->GetCamera();
//	Matrix ndcMat = CameraToNDC(cam);
//	Affine camMat = WorldToCamera(cam);
//	Matrix vp = ndcMat * camMat;
//	Point camEye = cam.Eye();
//	Vector4 camEyeVec4 = Vector4{ camEye.x, camEye.y, camEye.z, 1.f };
//
//
//
//	
//	if (InputManager::instance->IsPressed('z'))
//	{
//		sandParticle->Realloc(NUMSPAWNERS);
//		NUMPARTICLES = sandParticle->GetIndicesNum();
//		AddSandsInSpawnerPos(sandParticle->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr()
//			, spawnerPos->GetDeviceMemoryPtr(), sandParticle->GetLastIndexBeforeRealloc());
//
//		sandParticle->CopyDeviceToHost();
//
//		delete sandPosBuffer;
//		sandPosBuffer = new Buffer(GL_ARRAY_BUFFER, sandParticle->GetMemorySize(), GL_STATIC_DRAW, sandParticle->GetHostMemoryPtr());
//	}
//	if (InputManager::instance->IsPressed('x'))
//	{
//		land->Realloc(NUMSPAWNERS);
//		NUMLANDS = land->GetIndicesNum();
//		AddLandsInSpawnerPos(land->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr(), 
//			spawnerPos->GetDeviceMemoryPtr(), land->GetLastIndexBeforeRealloc());
//		
//		land->CopyDeviceToHost();
//
//		delete landPosBuffer;
//		landPosBuffer = new Buffer(GL_ARRAY_BUFFER, land->GetMemorySize(), GL_STATIC_DRAW, land->GetHostMemoryPtr());
//	}
//	if(InputManager::instance->IsPressed('b'))
//	{
//		waterParticle->Realloc(NUMSPAWNERS);
//		NUMWATERS = waterParticle->GetIndicesNum();
//
//		AddWatersInSpawnerPos(waterParticle->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr(),
//			spawnerPos->GetDeviceMemoryPtr(), waterParticle->GetLastIndexBeforeRealloc());
//		
//		waterParticle->CopyDeviceToHost();
//		delete waterPosBuffer;
//		waterPosBuffer = new Buffer(GL_ARRAY_BUFFER, waterParticle->GetMemorySize(), GL_STATIC_DRAW, waterParticle->GetHostMemoryPtr());
//	}
//	if (InputManager::instance->IsPressed('c'))
//	{
//		land->CopyDeviceToHost();
//		std::vector<Vertex> vertexDatas;
//		Land* lands = land->GetHostMemoryPtr();
//		for (int i = 0; i < NUMLANDS; ++i)
//		{
//			Vertex vertex;
//			vertex.position.x = static_cast<float>(lands[i].currGridIndex);
//			vertexDatas.push_back(vertex);
//		}
//		BinaryFileManager::SaveBinFile("sand", vertexDatas, false, false);
//	}
//	if(InputManager::instance->IsPressed('v'))
//	{
//		DeleteLands(land->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr());
//	}
//
//	int speed = 8;
//	if (InputManager::instance->IsPressed('j'))
//	{
//		left
//		MoveSpawner(sandGrid->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr(), -1 * speed, NUMSPAWNERS);
//	}
//	if (InputManager::instance->IsPressed('l'))
//	{
//		right
//		MoveSpawner(sandGrid->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr(), 1 * speed, NUMSPAWNERS);
//	}
//	if (InputManager::instance->IsPressed('k'))
//	{
//		up
//		MoveSpawner(sandGrid->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr(), -800 * speed, NUMSPAWNERS);
//	}
//	if (InputManager::instance->IsPressed('i'))
//	{
//		down
//		MoveSpawner(sandGrid->GetDeviceMemoryPtr(), spawnerPos->GetDeviceMemoryPtr(), 800 * speed, NUMSPAWNERS);
//	}
//
//	/*if(timer < 0.f)
//	{
//		CudaBuffer<SpawnerPos>* spawner;
//
//		if (isFirst)
//			spawner = firstContinuouslySpawn;
//		else
//			spawner = secondContinuouslySpawn;
//
//		sandParticle->Realloc(NUMCONTISPAWNERS);
//		NUMPARTICLES = sandParticle->GetIndicesNum();
//		AddSandsInSpawnerPos(sandParticle->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr()
//			, spawner->GetDeviceMemoryPtr(), sandParticle->GetLastIndexBeforeRealloc());
//
//		sandParticle->CopyDeviceToHost();
//
//		delete sandPosBuffer;
//		sandPosBuffer = new Buffer(GL_ARRAY_BUFFER, sandParticle->GetMemorySize(), GL_STATIC_DRAW, sandParticle->GetHostMemoryPtr());
//		
//		timer = 0.1f;
//	}*/
//	timer -= dt;
//
//
//	
//	SimulationUpdate(NUMPARTICLES, NUMGRIDS, NUMWATERS,
//		sandParticle->GetDeviceMemoryPtr(), waterParticle->GetDeviceMemoryPtr(),
//		sandGrid->GetDeviceMemoryPtr());
//
//	CopyFromDeviceToHost();
//
//	
//	sandPosBuffer->WriteData<ParticleSand>(sandParticle->GetHostMemoryPtr());
//	spawnerPosBuffer->WriteData<SpawnerPos>(spawnerPos->GetHostMemoryPtr());
//	landPosBuffer->WriteData<Land>(land->GetHostMemoryPtr());
//	waterPosBuffer->WriteData<ParticleWater>(waterParticle->GetHostMemoryPtr());
//
//	glEnable(GL_PROGRAM_POINT_SIZE);
//	glPointSize(10.f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.f, 0.f, 0.f, 1.f);
//	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
//	
//	Vector3 colorWhite(1.f);
//	Vector3 colorYellow(0.859375f, 0.75f, 0.54296875f);
//	Vector3 colorGreen(0.f, 0.5f, 0.0f);
//	Vector3 colorRed(0.5f, 0.0f, 0.0f);
//	Vector3 colorBlue(0.8313725490f, 0.9450980392f, 0.9764705882f);
//
//	render->Use();
//	render->SendUniformFloat("pointSize", 1.0f);
//	render->SendUniformMat("VP", &vp);
//
//	render->SendUniform3fv("color_val", &colorYellow, 1);
//
//	sandPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleSand), (GLvoid*)offsetof(ParticleSand, pos));
//	glEnableVertexAttribArray(0);
//
//	glDrawArrays(GL_POINTS, 0, NUMPARTICLES);
//
//	landPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Land), (GLvoid*)offsetof(Land, landPos));
//	glEnableVertexAttribArray(0);
//
//	render->SendUniform3fv("color_val", &colorGreen, 1);
//	glDrawArrays(GL_POINTS, 0, NUMLANDS);
//
//	spawnerPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpawnerPos), (GLvoid*)offsetof(SpawnerPos, pos));
//	glEnableVertexAttribArray(0);
//
//	render->SendUniform3fv("color_val", &colorRed, 1);
//
//	glDrawArrays(GL_POINTS, 0, NUMSPAWNERS);
//
//	waterPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleWater), (GLvoid*)offsetof(ParticleWater, pos));
//	glEnableVertexAttribArray(0);
//
//	render->SendUniform3fv("color_val", &colorBlue, 1);
//
//	glDrawArrays(GL_POINTS, 0, NUMWATERS);
//
//	firstContiPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpawnerPos), (GLvoid*)offsetof(SpawnerPos, pos));
//	glEnableVertexAttribArray(0);
//
//	render->SendUniform3fv("color_val", &colorBlue, 1);
//	glDrawArrays(GL_POINTS, 0, NUMCONTISPAWNERS);
//
//	secondContiPosBuffer->Bind();
//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpawnerPos), (GLvoid*)offsetof(SpawnerPos, pos));
//	glEnableVertexAttribArray(0);
//
//	render->SendUniform3fv("color_val", &colorRed, 1);
//	glDrawArrays(GL_POINTS, 0, NUMCONTISPAWNERS);
//
//	isFirst = !isFirst;
//}
//
//void Level11::UnLoad()
//{
//}
//
//void Level11::LoadMap()
//{
//	std::vector<Vertex> vertexDatas;
//	int check = 0;
//	bool isLoaded = BinaryFileManager::LoadDataFile("sand", vertexDatas, check);
//	
//	/*if(isLoaded)
//	{
//		delete land;
//
//		NUMLANDS = static_cast<int>(vertexDatas.size());
//
//		const int size = NUMLANDS * sizeof(int);
//		loadedLands = new CudaBuffer<int>(size);
//		int* loadedLandHost = loadedLands->GetHostMemoryPtr();
//
//		for (int i = 0; i < NUMLANDS; ++i)
//		{
//			loadedLandHost[i] = static_cast<int>(vertexDatas[i].position.x);
//		}
//		loadedLands->CopyHostToDevice();
//		landMemSize = NUMLANDS * sizeof(Land);
//		land = new CudaBuffer<Land>(landMemSize);
//
//		LoadLands(loadedLands->GetDeviceMemoryPtr(), sandGrid->GetDeviceMemoryPtr(), land->GetDeviceMemoryPtr(), NUMLANDS);
//		land->CopyDeviceToHost();
//	}*/
//
//}
//
//void Level11::CopyFromDeviceToHost()
//{
//	sandParticle->CopyDeviceToHost();
//	sandGrid->CopyDeviceToHost();
//	land->CopyDeviceToHost();
//	spawnerPos->CopyDeviceToHost();
//	waterParticle->CopyDeviceToHost();
//	firstContinuouslySpawn->CopyDeviceToHost();
//	secondContinuouslySpawn->CopyDeviceToHost();
//}
//
//void Level11::CopyFromHostToDevice()
//{
//	sandParticle->CopyHostToDevice();
//	sandGrid->CopyHostToDevice();
//	land->CopyHostToDevice();
//	spawnerPos->CopyHostToDevice();
//	waterParticle->CopyHostToDevice();
//	firstContinuouslySpawn->CopyHostToDevice();
//	secondContinuouslySpawn->CopyHostToDevice();
//}
