#pragma once
#include "Affine.h"

struct WaterParticle
{
	int id;
	float density;
	float lambda;
	Vector3 pos;
	Vector3 vel;
	Vector3 force;
};

struct ParticleVec4
{
	Vector4 velocity;
	Vector4 predictedPos;
};

struct ParticleVec42
{
	Vector4 pos;
	Vector4 velocity;
	Vector4 force;
	Vector4 predictedPos;
};

struct ParticleVal
{
	float density;
	float lambda;
	int id;
};

const int neighborCount = 100;
const int binCountX = 16;
const int binCountY = 20;
const int binCountZ = 20;
const int binCount = binCountX * binCountY * binCountZ;

struct Neighbors
{
	int neighbor[neighborCount];
};

typedef struct WaterParticle WaterParticle;
typedef struct WaterParticle* pWaterParticle;

struct Indices
{
	int ix, iy, iz;
};

struct Bubble {
	int type;
	float radius;
	Vector4 pos;
	Vector4 vel;
	float lifetime;
};

enum BubbleType
{
	None = -1,
	Spray = 0,
	Bubble = 1,
	Foam = 2
};

struct BubbleVec4
{
	Vector4 bubblePos;
	Vector4 bubbleVel;
};

struct CheckIndices
{
	int x, y, z, w;
};