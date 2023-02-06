#pragma once
#include "Affine.h"


enum GridStatus
{
	Empty,
	FilledWithSand,
	FilledWithLand,
	FilledWithWater,
};

struct ParticleGrid
{
	Vector2 gridPos;
	int index;
	GridStatus status;
	int particleIndex;
	int landIndex;
	int predictedMoveInWaterParticleIndex;
};

struct SpawnerPos 
{
	Vector2 pos;
	int currGridIndex;
};

struct Land 
{
	Vector2 landPos;
	int currGridIndex;
};

struct ParticleSand
{
	Vector2 pos;
	ParticleGrid* currGrid;
};

struct ParticleWater
{
	Vector2 pos;
	int currGridIndex;
	int predictMoveInGridIndex;
};