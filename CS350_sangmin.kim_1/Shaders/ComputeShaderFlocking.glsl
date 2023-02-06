#version 430 core

layout(local_size_x = 256) in;

uniform float closestAllowedDistance2 = 50.0;
uniform float rule1Weight = 0.18;
uniform float rule2Weight = 0.05;
uniform float rule3Weight = 0.3;
uniform float rule4Weight = 0.002;
uniform vec3 goal;
uniform float timeStep = 0.5;

struct flockMember
{
	vec3 position;
	vec3 velocity;
};

layout(std430, binding = 0) buffer memberIn
{
	flockMember member[];
}inputData;

layout(std430, binding = 1) buffer memberOut
{
	flockMember member[];
}outputData;

shared flockMember sharedMember[gl_WorkGroupSize.x];

vec3 rule1(vec3 myPosition, vec3 myVelocity, vec3 theirPosition, vec3 theirVelocity)
{
	vec3 dir = myPosition - theirPosition;

	if(dot(dir, dir) < closestAllowedDistance2)
	{
		return dir;
	}
	return vec3(0.0);
}

vec3 rule2(vec3 myPosition, vec3 myVelocity, vec3 theirPosition, vec3 theirVelocity)
{
	vec3 dir = theirPosition - myPosition;
	vec3 dv = theirVelocity - myVelocity;
	return dv / (dot(dir, dir) + 10.0);
}

void main(void)
{
	uint i,j;
	int globalId = int(gl_GlobalInvocationID.x);
	int localId = int(gl_LocalInvocationID.x);

	flockMember me = inputData.member[globalId];
	flockMember newMe;
	vec3 acceleration = vec3(0.0);
	vec3 flockCenter = vec3(0.0);

	for(i = 0 ; i < gl_NumWorkGroups.x; ++i)
	{
		flockMember them = inputData.member[i * gl_WorkGroupSize.x + localId];

		sharedMember[localId] = them;

		memoryBarrierShared();
		barrier();

		for(j = 0; j < gl_WorkGroupSize.x; ++j)
		{
			them = sharedMember[j];
			flockCenter += them.position;

			if(i * gl_WorkGroupSize.x + j != globalId)
			{
				acceleration += rule1(me.position, me.velocity, them.position, them.velocity) * rule1Weight;

				acceleration += rule2(me.position, me.velocity, them.position, them.velocity) * rule2Weight;
			}
		}
		barrier();
	}
	flockCenter /= float(gl_NumWorkGroups.x * gl_WorkGroupSize.x);
	newMe.position = me.position + me.velocity * timeStep;
	acceleration += normalize(goal - me.position) * rule3Weight;
	acceleration += normalize(flockCenter - me.position) * rule4Weight;
	newMe.velocity = me.velocity + acceleration * timeStep;

	if(length(newMe.velocity) > 10.0)
	{
		newMe.velocity = normalize(newMe.velocity) * 10.0;
	}

	newMe.velocity = mix(me.velocity, newMe.velocity, 0.4);
	outputData.member[globalId] = newMe;
}