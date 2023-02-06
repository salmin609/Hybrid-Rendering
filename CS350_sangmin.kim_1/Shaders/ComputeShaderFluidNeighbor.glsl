#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;
//const int particleNum = 30  * 30  * 30 ;
uniform int particleNum;

const int neighborCount = 100;
struct ParticleNeighbors
{
	int neighborIndices[neighborCount];
};

layout(std430, binding = 0) buffer poses
{
	vec4 pos[];
};

layout(std430, binding = 1) buffer neighbors
{
	ParticleNeighbors neighbor[];
};

layout(std430, binding = 2) buffer neighborsCheckCount
{
	int neighborCheckCount[];
};

const float h = 0.14f;

//neighbor
const float gap = h; //0.14f

void AddNeighborIndex(int i)
{
	uint gid = gl_GlobalInvocationID.x;
	neighbor[gid].neighborIndices[neighborCheckCount[gid]] = i;
	neighborCheckCount[gid]++;
	
	//neighbor[i].neighborIndices[neighborCheckCount[i]] = int(gid);
	//neighborCheckCount[i]++;

	//barrier();
	//memoryBarrierShared();
	//neighborCheckCount[i]++;

}

void CalculateNeighboringIndices()
{
	uint gid = gl_GlobalInvocationID.x;
	vec4 posVal = pos[gid];

	for(int i = 0; i < particleNum; ++i)
	{

		vec4 otherPosVal = pos[i];

		
		//check
		float xBoundaryMin = posVal.x - gap;
		float xBoundaryMax = posVal.x + gap;
		float yBoundaryMin = posVal.y - gap;
		float yBoundaryMax = posVal.y + gap;
		float zBoundaryMin = posVal.z - gap;
		float zBoundaryMax = posVal.z + gap;

		if(otherPosVal.x > xBoundaryMin && otherPosVal.x < xBoundaryMax && 
		otherPosVal.y > yBoundaryMin && otherPosVal.y < yBoundaryMax &&
		otherPosVal.z > zBoundaryMin && otherPosVal.z < zBoundaryMax)
		{
			AddNeighborIndex(i);
		}
		

		if(neighborCheckCount[gid] > neighborCount - 1)
			break;
		
	}

}

void main()
{
	CalculateNeighboringIndices();

	//uint gid = gl_GlobalInvocationID.x;

	//neighborCheckCount[int(gid)] = 0;
}
