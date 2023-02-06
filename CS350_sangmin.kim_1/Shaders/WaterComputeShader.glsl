#version 430 core

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) buffer
bufferInpos
{
	vec4 inPos[];
};

layout(binding = 1) buffer
bufferInVel
{
	vec4 inVelocity[];
};

layout(binding = 2) buffer
bufferOutPos
{
	vec4 outPos[];
};

layout(binding = 3) buffer
bufferOutVelocity
{
	vec4 outVelocity[];
};

uniform float dt;
uniform int numParticles;
const float gAccel = 9.8;

void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if(index > numParticles)
	{
		return;
	}
	
	vec4 pos = inPos[index];
	vec4 vel = inVelocity[index];
	vec4 newVel = vel;
	
	if(pos.y > 0.001)
	{	
		pos.xyz += vel.xyz * dt;
		newVel -= gAccel * dt;
	}

	if(pos.y <= 0.01)
	{
		newVel.xyz *= 0.2;
		newVel.y *= -1;
	}
	
	//wall
	const float wallDamping = 0.8;
	
	if(pos.x > 300)
	{
		pos.x = 299.9;
		newVel.x *= -wallDamping;
	}
	else if(pos.x < -300)
	{
		pos.x = -299.9;
		newVel.x *= -wallDamping;
	}
	
	if(pos.z > 300)
	{
		pos.z = 299.9;
		newVel.z *= -wallDamping;
	}
	else if(pos.z < -300)
	{
		pos.z = -299.9;
		newVel.z *= -wallDamping;
	}

	pos.w = length(newVel);
	outPos[index] = pos;
	outVelocity[index] = newVel;
}