#pragma once
#include "Affine.h"
#include "State.h"
class VAO;
class Shader;
class Buffer;
const int sphereCount = 20;
class Level8 : public State
{
public:
	Level8();
	~Level8();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
private:
	Shader* renderShader;
	Shader* computeShader;
	Buffer* positionBuffer[2];
	Buffer* velocityBuffer[2];
	Buffer* colorBuffer;
	int particleCount;
	unsigned m_NumWorkGroups[3];
	VAO* drawVao[2];
	struct ParticlePos
	{
		float x;
		float y;
		float z;
		float w;          //Holds the speed so when we get this in the pixel shader we can color our particles based on that.
	};
	

	struct Spheres_t
	{
		Vector3	centers[sphereCount];
		float radii[sphereCount];
	};
	Spheres_t spheres;


	//ParticleVelocity are only used in the compute shader.
	//This is why I keep the  fTimeToLive here.
	struct ParticleVelocity
	{
		float vx;
		float vy;
		float vz;
		float fTimeToLive; //Remaining time. 
	};
	ParticlePos* particlePositions;
	ParticleVelocity* particleVelocities;
	Vector3* particleColor;
	int index = 0;
};