#include "Particle.h"
#include "Graphic.h"
#include "RandomNumGenerator.h"

Particle::Particle()
{
	isInstancing = true;
	Particle::SetVertices();
}

void Particle::SetVertices()
{
	timer = 2.f;
	instancingNum = 0;
	particleNum = 1000;

	for (size_t i = 0; i < particleNum; ++i)
	{
		const float randomx = RandomNumber::instance->RandomFloat(0.f, 1.f);
		const float randomy = RandomNumber::instance->RandomFloat(0.f, 1.f);
		const float randomz = RandomNumber::instance->RandomFloat(0.f, 1.f);

		const float randomDirX = RandomNumber::instance->RandomFloat(0.f, 10.f);
		const float randomDirY = RandomNumber::instance->RandomFloat(0.f, 10.f);
		const float randomDirZ = RandomNumber::instance->RandomFloat(0.f, 10.f);

		const float randomRotX = RandomNumber::instance->RandomFloat(0.f, 10.f);
		const float randomRotY = RandomNumber::instance->RandomFloat(0.f, 10.f);
		const float randomRotZ = RandomNumber::instance->RandomFloat(0.f, 10.f);

		ParticleInstance data{ 2.f, Vector3{
			randomx,
			randomy,
			randomz
		},
		Vector3{
			randomDirX,
			randomDirY,
			randomDirZ
		},
		Vector3{
			randomRotX,
			randomRotY,
			randomRotZ
		} };
		
		particles.push_back(data);
	}
	InitializeColoredParticle(shaderParticleColoredVertex, shaderParticleColoredFragment);
}

void Particle::MoveParticle(float dt)
{
	const float speed = 2.f;
	const float rotateSpeed = 10.f;
	const int randomIndex = RandomNumber::instance->RandomInt(10);
	instancingNum += randomIndex;

	if (instancingNum > particleNum)
	{
		instancingNum = particleNum;
	}
	std::vector<Vector3> dirVec;
	std::vector<Vector3> rotateVec;
	std::vector<Vector3> colorVec;

	for (int i = 0; i < instancingNum; ++i)
	{
		ParticleInstance& particle = particles[i];

		particle.life -= dt;
		if (particle.life < 0.f)
		{
			instancingNum--;
			particle.life = 2.f;
			particle.dir.x = RandomNumber::instance->RandomFloat(0.f, 10.f);
			particle.dir.y = RandomNumber::instance->RandomFloat(0.f, 10.f);
			particle.dir.z = RandomNumber::instance->RandomFloat(0.f, 10.f);

			particle.rotate.x = RandomNumber::instance->RandomFloat(0.f, 10.f);
			particle.rotate.y = RandomNumber::instance->RandomFloat(0.f, 10.f);
			particle.rotate.z = RandomNumber::instance->RandomFloat(0.f, 10.f);

			particle.color.x = RandomNumber::instance->RandomFloat(0.f, 1.f);
			particle.color.y = RandomNumber::instance->RandomFloat(0.f, 1.f);
			particle.color.z = RandomNumber::instance->RandomFloat(0.f, 1.f);
		}
		else
		{
			particle.dir.x += (particle.dir.x * speed * dt);
			particle.dir.y += (particle.dir.y * speed * dt);
			particle.dir.z += (particle.dir.z * speed * dt);

			particle.rotate.x += (particle.rotate.x * rotateSpeed * dt);
			particle.rotate.y += (particle.rotate.y * rotateSpeed * dt);
			particle.rotate.z += (particle.rotate.z * rotateSpeed * dt);
		}
		dirVec.push_back(particle.dir);
		rotateVec.push_back(particle.rotate);
		colorVec.push_back(particle.color);
	}

	glBindBuffer(GL_ARRAY_BUFFER, matrixId);

	glBufferSubData(GL_ARRAY_BUFFER, 0,
		dirVec.size() * sizeof(Vector3),
		dirVec.data());

	glBindBuffer(GL_ARRAY_BUFFER, rotateId);

	glBufferSubData(GL_ARRAY_BUFFER, 0,
		rotateVec.size() * sizeof(Vector3),
		rotateVec.data());

	glBindBuffer(GL_ARRAY_BUFFER, color_id);

	glBufferSubData(GL_ARRAY_BUFFER, 0,
		colorVec.size() * sizeof(Vector3),
		colorVec.data());
}
