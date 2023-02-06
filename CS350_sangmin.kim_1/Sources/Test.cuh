#ifndef TEST_CUH
#define TEST_CUH
#include "SandParticle.h"


void SetPosition(ParticleSand* pos);
void Init(int particleNum, int gridNum, int spawnerNum, int contiSpawnerNum, ParticleSand* particle, ParticleGrid* grid, Land* land, SpawnerPos* spawners,
	SpawnerPos* firstContiSpawner, SpawnerPos* secondContiSpawner);
void SetLands(std::vector<int>& landStartRandomIndices, ParticleSand* particle, ParticleGrid* grid, Land* land);
void SimulationUpdate(int particleNum, int gridNum, int waterNum, ParticleSand* particle, ParticleWater* water, ParticleGrid* grid);
void AddSandsInSpawnerPos(ParticleSand* particle, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex);
void AddSandsInContSpawnerPos(ParticleSand* particle, ParticleGrid* grid, Vector2* spawners, int lastIndex);
void AddLandsInSpawnerPos(Land* lands, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex);
void AddWatersInSpawnerPos(ParticleWater* waters, ParticleGrid* grid, SpawnerPos* spawners, int lastIndex);
void MoveSpawner(ParticleGrid* grid, SpawnerPos* spawners, int dir, int spawnerCount);
void LoadLands(int* landGridIndices, ParticleGrid* grid, Land* land, int landsNum);
void DeleteLands(Land* lands, ParticleGrid* grids, SpawnerPos* spawners);
#endif