#pragma once
#include <vector>
#include "Affine.h"
#include "CudaBuffer.hpp"
#include "State.h"
#include "SandParticle.h"

class Model;
class LoadedObj;
class VAO;
class Buffer;
class Shader;
class TgaTexture;
class FluidCompute;

class Level11 : public State
{
public:
	Level11();
	~Level11();

	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
	void LoadMap();
private:
	void CopyFromDeviceToHost();
	void CopyFromHostToDevice();
	Shader* render;
	Shader* renderOnePoint;
	Buffer* sandPosBuffer;
	Buffer* gridPosBuffer;
	Buffer* landPosBuffer;
	Buffer* spawnerPosBuffer;
	Buffer* waterPosBuffer;
	Buffer* firstContiPosBuffer;
	Buffer* secondContiPosBuffer;

	//CudaBuffer<ParticleSand>* sandParticle;
	//CudaBuffer<ParticleGrid>* sandGrid;
	//CudaBuffer<Land>* land;
	//CudaBuffer<SpawnerPos>* spawnerPos;
	//CudaBuffer<int>* loadedLands;
	//CudaBuffer<ParticleWater>* waterParticle;
	//CudaBuffer<SpawnerPos>* firstContinuouslySpawn;
	//CudaBuffer<SpawnerPos>* secondContinuouslySpawn;

	float timer = 1.f;
	
	int NUMPARTICLES = 0;
	int particleMemSize = NUMPARTICLES * sizeof(ParticleSand);
	
	int NUMGRIDS = 256 * 2000;
	int gridMemSize = NUMGRIDS * sizeof(ParticleGrid);

	int NUMLANDS = 256 * 4;
	int landMemSize = NUMLANDS * sizeof(Land);

	int NUMSPAWNERS = 64;
	int spawnerMemSize = NUMSPAWNERS * sizeof(SpawnerPos);

	int NUMWATERS = 0;
	int waterMemSize = NUMWATERS * sizeof(ParticleWater);

	int NUMCONTISPAWNERS = 256;
	int contiSpawnerMemSize = NUMCONTISPAWNERS * sizeof(SpawnerPos);

	bool isFirst = true;
};
