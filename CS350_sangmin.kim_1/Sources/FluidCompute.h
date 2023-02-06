#pragma once
#include <thread>
#include <vector>
#include "Affine.h"
#include "FluidGrid.h"

class FluidCompute
{
public:
	FluidCompute();
	~FluidCompute();
	void Update();
	int ParticleTotalNum();
	float* ParticlePositionData();
	WaterParticle* Particles();
	std::vector<Vector3> ParticlePos();
	int PxNum();
	int PyNum();
	int PzNum();
private:
	float GetC(int i);
	float CalculateW(int i, int j);
	Vector3 CalculateDW(int i, int j);
	void PredictPosition();
	void UpdateVelocityPos();
	void CollisionDetection();
	void ComputeDensity();
	void ComputeDeltaP();
	void ComputeLambda();
	std::vector<std::thread> threads;
	std::vector<WaterParticle> bufferPart;
	
	pWaterParticle* particles = nullptr;
	std::vector<Vector3> particlesPos;
	FluidGrid* grid;
	
	
	const float pDist = 0.09f;
	const int pxNum = 25;
	const int pyNum = 25;
	const int pzNum = 25;
	const int pTotalNum = pxNum * pyNum * pzNum;
	const float pStartX = -pDist * pxNum / 2.f;
	const float pStartY = -pDist * pyNum / 2.f;
	const float pStartZ = -pDist * pzNum / 2.f;

	float d = 1.f;
	float wxMin = -d;
	float wxMax = 2.f * d;
	float wyMin = -2.f * d;
	float wyMax = d;
	float wzMin = -d;
	float wzMax = d;
	
	const float pRadius = 20.f;
	const float pMass = 1.25e-5f;
	
	const int threadNum = 4;
	const int workLoad = pTotalNum / threadNum;

	const float gravity = 9.8f;
	const float tStep = 1.0f / 24;
	const float PI = 3.1415926f;

	const float h = 0.14f;
	const float hsqr = h * h;
	const float sigma = 0.1f;
	const float wk = 315.f / static_cast<float>(64 * PI * pow(h, 9));
	const float dwk = 15.f / static_cast<float>(PI * pow(h, 6));
	const float scorrk = wk * static_cast<float>(pow(0.99 * hsqr, 3));
	const float pDensity0 = 1000.f;
	
	const float minta = 10.f;
	const float maxta = 45.f;
	const float minK = 0.1f;
	const float maxK = 0.3f;
	const int kta = 50;
	const float maxLifeTime = 3.f;
	const int SPRAY = 0;
	const int BUBBLE = 1;
	const int FOAM = 2;
	const int LIMIT1 = 5;
	const int LIMIT2 = 50;
	const float kb = 0.8f;
	const float kd = 0.1f;
	const float mass = 0.1f;
};
