#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout (binding = 0) buffer
bufferInpos{
	vec4 inPos[];
};

layout (binding = 1) buffer
bufferInVelocity{
	vec4 inVelocity[];
};

layout (binding = 2) buffer
bufferOutPos{
	vec4 outPos[];
};

layout (binding = 3) buffer
bufferOutVelocity{
	vec4 outVelocity[];
};

uniform float dt;
const int sphereCount = 20;
uniform vec3 sphereOffsets[sphereCount];
uniform float sphereRadius[sphereCount];

uniform int gNumParticles;
const float gAccel = 9.8;

float DistanceFieldCircle(vec3 p, vec3 sphereOffset, float sphereRadius)
{
	return length(sphereOffset - p) - sphereRadius;
}
float rand(vec2 co)
{
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453) * 1000.0;
}

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if(index > gNumParticles)
	{
		return;
	}

	vec4 particlePos = inPos[index];
	vec4 particleVel = inVelocity[index];

	vec4 newParticleVelocity = particleVel;

	if(particlePos.y <= 0.001)
	{
		float rand1 = rand(particlePos.xz);
		float rand2 = rand(particlePos.zx);

		particlePos.x = -700.0 + mod(rand1 * 10, 1400.0);
		particlePos.y = 500.0 + mod(rand1, 50);
		particlePos.z = -700.0 + mod(rand2 * 10, 1400.0);

		newParticleVelocity.x = -5.0 + mod(rand2 * 100.0, 10.0);
		newParticleVelocity.y = -5.0 + mod(rand2 * 100.0, 10.0);
		newParticleVelocity.z = -5.0 + mod(rand1 * 100.0, 10.0);
		newParticleVelocity.w = 10.0;
	}
	else
	{
		particlePos.xyz = particlePos.xyz + particleVel.xyz * dt;
		newParticleVelocity.y = newParticleVelocity.y - gAccel * dt;
	}

	if(particlePos.y <= 0.01)
	{
		newParticleVelocity.xyz *= 0.2;
		newParticleVelocity.y *= -1.0;
	}

	const float wallDamping = 0.5;
	if(particlePos.x > 700.0)
	{
		particlePos.x = 699.9;
		newParticleVelocity.x *= -wallDamping;
	}
	else if(particlePos.x < -700.0)
	{
		particlePos.x = -699.9;
		newParticleVelocity.x *= -wallDamping;
	}
	else if(particlePos.z > 700.0)
	{
		particlePos.z = 699.9;
		newParticleVelocity.z *= -wallDamping;
	}
	else if(particlePos.z < -700)
	{
		particlePos.z = -699.9;
		newParticleVelocity.z *= -wallDamping;
	}

	float minDist = 10000.0;
	int closestSphereIndex = -1;

	for(int i = 0 ; i < sphereCount; ++i)
	{
		vec3 sphereCenter = sphereOffsets[i];
		vec3 localPosition = particlePos.xyz - sphereCenter;

		float dist = DistanceFieldCircle(particlePos.xyz, sphereCenter, sphereRadius[i]);

		if(dist < minDist && dist < 0)
		{
			minDist = dist;
			closestSphereIndex = i;
		}
	}
	if(closestSphereIndex != -1)
	{
		vec3 localPosition = vec3(particlePos.x, particlePos.y, particlePos.z) - sphereOffsets[closestSphereIndex];
		vec3 ReflectionNormal = normalize(localPosition);
		
		//Reflect our speed
		newParticleVelocity.xyz = reflect(newParticleVelocity.xyz,ReflectionNormal);
		if(abs(newParticleVelocity.x) > 2.0)
            newParticleVelocity.x *= 0.2;
        if(abs(newParticleVelocity.y) > 2.0)
            newParticleVelocity.y *= 0.2;
        if(abs(newParticleVelocity.z) > 2.0)
            newParticleVelocity.z *= 0.2;

		//Move the particle away from the collision just a bit.
		particlePos.xyz = sphereOffsets[closestSphereIndex] + ReflectionNormal * ( sphereRadius[closestSphereIndex] + 0.1);
	}

	particlePos.w = length(newParticleVelocity);

	newParticleVelocity.w -= dt;

	outPos[index] = particlePos;
	outVelocity[index] = newParticleVelocity;
}