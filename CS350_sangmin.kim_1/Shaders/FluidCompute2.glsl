#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

const int pxNum = 20;
const int pyNum = 20;
const int pzNum = 20;

const int pTotalCount = pxNum * pyNum * pzNum;
const int neighborCount = 3 * 3 * 3;

const float PI = 3.1415926f;
const float tStep = 0.02f;
const float gravity = -9.82f;

//Dimensions
const vec3 halfDimensions = vec3(0.8f, 1.0f, 1.0f);
const vec3 halfDiff = vec3(0.015f);
const float binSize = 0.1f;
const vec4 binCount3D = vec4(16, 20, 20, 0);
const int binCount = int(binCount3D.x * binCount3D.y * binCount3D.z);

const float kernelRadius = 0.1f;
const float kBoundsDensity = 50.0f;
const float restDensity  = 8000.0f;
const float fluidepsilon = 1000.0f;
const float fluidK		 = 0.0005f;
const float fluidDelta_q = 0.03f;
const int	fluidN		 = 4;
const float fluidK_vc    = 75.0f;
const float fluidC       = 0.05f;

#define EPSILON 0.0001f
#define ONE_OVER_SQRT_OF_3 0.577350f
#define MAX_DELTA_PI vec3(0.1f, 0.1f, 0.1f)

struct ParticleNeighbors
{
	int neighborIndices[neighborCount];
};

struct ParticleVec4
{
	vec4 pos;
	vec4 velocity;
	vec4 force;
	vec4 predictedPos;
};
struct ParticleVal
{
	float density;
	float lambda;
	int id;
};
struct BubbleVec4
{
	vec4 bubblePos;
	vec4 bubbleVel;
};


layout(std430, binding = 0) buffer particleVec4
{
	ParticleVec4 particleInfoVec4[];
};

layout(std430, binding = 1) buffer particleFloat
{
	ParticleVal particleInfoValue[];
};

layout(std430, binding = 2) buffer neighbors
{
	ParticleNeighbors neighbor[];
};

layout(std430, binding = 3) buffer poses
{
	vec4 pos[];
};

layout(std430, binding = 4) buffer ind
{
	ivec4 indicesVal[];
};



float euclidean_distance2(vec3 r) 
{
    return r.x * r.x + r.y * r.y + r.z * r.z;
}

float euclidean_distance(vec3 r) 
{
    return sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
}

float calc_bound_density_contribution(float dx_, float kernelRadius_) {
    if (dx_ > kernelRadius_) {
        return 0.0f;
    }

    if (dx_ <= 0.0f) {
        return (2 * PI / 3);
    }

    return (2 * PI / 3) * pow(kernelRadius_ - dx_, 2) * (kernelRadius_ + dx_);
}

float Wpoly6(vec3 r, float h);
vec3 grad_Wspiky(vec3 r, float h);
ivec3 GetBinId3D(vec3 pos);
uint GetBinId(ivec3 binId);
void TimeStep();
void ClipPredictedPosition();
void InsertParticles();
int FindNextEmptyNeighboringIndex(int binIndex);
void CalcDensity();
void CalcLambda();
void CalcDeltaPiAndUpdate();
void ReCalcVelocities();
void CalcCurls();
void ApplyVortAndViscXSPH();
int GetBinIdFromPredictedPos(int index);

int index = int(gl_GlobalInvocationID.x);

float Wpoly6(vec3 r, float h)
{
	const float tmp = h * h - euclidean_distance2(r);

	if(tmp < EPSILON)
		return 0.0f;

	  return (315.0f / (64.0f * PI * pow(h, 9))) * pow((tmp), 3);
}
vec3 grad_Wspiky(vec3 r, float h) 
{
    const float radius2 = euclidean_distance2(r);
    if (radius2 >= h * h) {
        return vec3(0.f);
    }
    if (radius2 <= EPSILON) {
        return vec3(0.f);
    }

    const float radius = sqrt(radius2);
    const float kernel_constant = - (15 / (PI * pow(h, 6))) * 3 * pow(h - radius, 2) / radius;

    return vec3(kernel_constant * r.x,
                kernel_constant * r.y,
                kernel_constant * r.z);
}


ivec3 GetBinId3D(vec3 pos)
{
	vec3 tmp = (pos + halfDimensions) / binSize;
	ivec3 indices = ivec3(floor(tmp));

	//indicesVal[index].xyz = indices;
	
	return clamp(indices, ivec3(0), ivec3(binCount3D.x - 1, binCount3D.y - 1, binCount3D.z - 1));
}
uint GetBinId(ivec3 binId)
{
	return uint(binId.x + binCount3D.x * binId.y + binCount3D.x * binCount3D.y * binId.z);
}

int FindNextEmptyNeighboringIndex(int binIndex)
{
	ParticleNeighbors neighbors = neighbor[binIndex];
	int neighborIndex = 0;

	while(neighbors.neighborIndices[neighborIndex] != -1)
	{
		if(neighborIndex > neighborCount - 1)
			return neighborCount - 1;
		
		neighborIndex++;
	}
	return neighborIndex;
}


void TimeStep()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	vec4 velVal = vec4Val.velocity;
	vec4 predictedVal = vec4Val.predictedPos;
	vec4 posVal = vec4Val.pos;

	velVal.y = velVal.y - (tStep * gravity);
	//velVal.y = -gravity;
	predictedVal = posVal + tStep * velVal;

	particleInfoVec4[index].predictedPos = predictedVal;
	//particleInfoVec4[index].velocity = velVal;
	//pos[index] = predictedVal;

}

void ClipPredictedPosition()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	vec3 predictedVal = vec3(vec4Val.predictedPos);

	predictedVal = clamp(vec3(predictedVal), -halfDimensions + halfDiff, halfDimensions - halfDiff);

	particleInfoVec4[index].predictedPos = vec4(predictedVal, 1.f);
	//pos[index] = particleInfoVec4[index].predictedPos;
}

void InsertParticles()
{
	int binId = GetBinIdFromPredictedPos(index);
	int emptyNeighborIndex = FindNextEmptyNeighboringIndex(int(binId));

	if(emptyNeighborIndex != -1 || emptyNeighborIndex != neighborCount - 1)
	{
		neighbor[binId].neighborIndices[emptyNeighborIndex] = index;
	}
}

int GetBinIdFromPredictedPos(int index)
{
	ParticleVec4 vec4Val = particleInfoVec4[index];

	vec3 predictedVal = vec3(vec4Val.predictedPos);
	const uint binId = GetBinId(GetBinId3D(predictedVal));

	return int(binId);
}

void CalcDensity()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	vec4 thisPos = vec4Val.predictedPos;
	float density = 0.f;

	int binId = GetBinIdFromPredictedPos(index);
	int emptyIndex = FindNextEmptyNeighboringIndex(int(binId));

	ParticleNeighbors neighbors = neighbor[binId];

	for(int i = 0 ; i < emptyIndex; ++i)
	{
		int neighborIndex = neighbors.neighborIndices[i];
		indicesVal[index].x = neighborIndex;

		ParticleVec4 neighborvec4Val = particleInfoVec4[neighborIndex];
		
		density = density + Wpoly6(vec3(neighborvec4Val.predictedPos - thisPos), kernelRadius);
	}

	float bDensity = 0.0f;

	bDensity = bDensity + calc_bound_density_contribution(thisPos.x + halfDimensions.x, kernelRadius);
    // x-right
    bDensity = bDensity + calc_bound_density_contribution(halfDimensions.x - thisPos.x, kernelRadius);
    // y-down
    bDensity = bDensity + calc_bound_density_contribution(thisPos.y + halfDimensions.y, kernelRadius);
    // y-up
    bDensity = bDensity + calc_bound_density_contribution(halfDimensions.y - thisPos.y, kernelRadius);
    // z-near
    bDensity = bDensity + calc_bound_density_contribution(thisPos.z + halfDimensions.z, kernelRadius);
    // z-far
    bDensity = bDensity + calc_bound_density_contribution(halfDimensions.z - thisPos.z, kernelRadius);

	particleInfoValue[index].density = density + kBoundsDensity * bDensity;
}

void CalcLambda()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	ParticleVal valVal = particleInfoValue[index];
	vec4 thisPos = vec4Val.predictedPos;
	float thisDensity = valVal.density;
	const float Ci = thisDensity / restDensity - 1;

	float sumOfSquaredGradients = 0.0f;
	vec3 grad_Ki = vec3(0.0f);
	vec3 k_position = vec3(0.0f);
	vec3 tmp_grad = vec3(0.0f);

	int thisBinId = GetBinIdFromPredictedPos(index);
	int binEmptySlot = FindNextEmptyNeighboringIndex(thisBinId);
	ParticleNeighbors neighbors = neighbor[thisBinId];
	grad_Ki = vec3(0.0f);

	for(int i = 0 ; i < binEmptySlot; ++i)
	{
		int neighborIndex = neighbors.neighborIndices[i];

		ParticleVec4 neighborvec4Val = particleInfoVec4[neighborIndex];

		
		ParticleVec4 val = particleInfoVec4[neighborIndex];
		k_position = vec3(val.predictedPos);

		tmp_grad = grad_Wspiky(vec3(thisPos) - k_position, kernelRadius);
		grad_Ki += tmp_grad;

		if(neighborIndex != index)
		{
			sumOfSquaredGradients = sumOfSquaredGradients + 
										tmp_grad.x * tmp_grad.x + 
										tmp_grad.y * tmp_grad.y + 
										tmp_grad.z * tmp_grad.z;
		}	
	}

	sumOfSquaredGradients = sumOfSquaredGradients + 
											tmp_grad.x * tmp_grad.x + 
											tmp_grad.y * tmp_grad.y + 
											tmp_grad.z * tmp_grad.z;

	const float lambda = -Ci / ((sumOfSquaredGradients / pow(restDensity, 2)) + fluidepsilon);
	particleInfoValue[index].lambda = lambda;
}

void CalcDeltaPiAndUpdate()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	ParticleVal valVal = particleInfoValue[index];
	vec3 thisPos = vec3(vec4Val.predictedPos);
	float thisDensity = valVal.density;
	float thisLambda = valVal.lambda;

	vec3 k_position = vec3(0.0);
	float s_corr = 0.0f;
	vec3 deltaPi = vec3(0.0);


	int thisBinId = GetBinIdFromPredictedPos(index);
	int binEmptySlot = FindNextEmptyNeighboringIndex(thisBinId);
	ParticleNeighbors neighbors = neighbor[thisBinId];

	for(int i = 0 ; i < binEmptySlot; ++i)
	{
		int neighborIndex = neighbors.neighborIndices[i];

		ParticleVec4 neighborvec4Val = particleInfoVec4[neighborIndex];
		
		ParticleVec4 Nval = particleInfoVec4[neighborIndex];
		ParticleVal NvalVal = particleInfoValue[neighborIndex];

		k_position = vec3(Nval.predictedPos);

		s_corr = -fluidK * pow(
		Wpoly6(vec3(thisPos) - k_position, kernelRadius) /
		Wpoly6(vec3(ONE_OVER_SQRT_OF_3 * fluidDelta_q), kernelRadius), fluidN);

		deltaPi = deltaPi + (thisLambda + NvalVal.lambda + s_corr) 
		* grad_Wspiky(vec3(thisPos) - k_position, kernelRadius);
	}

	deltaPi = deltaPi / restDensity;

	deltaPi = clamp(deltaPi, -MAX_DELTA_PI, MAX_DELTA_PI);

	particleInfoVec4[index].predictedPos = vec4(thisPos + deltaPi, 1.f);
}
void ReCalcVelocities()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	vec3 currentPos = vec3(vec4Val.predictedPos);
	vec3 prevPos  = vec3(vec4Val.pos);

	vec3 newVel = (currentPos - prevPos) * tStep;

	particleInfoVec4[index].velocity = vec4(newVel, 0.0);
}

void CalcCurls()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	vec3 thisPos = vec3(vec4Val.predictedPos);
	vec3 thisVel = vec3(vec4Val.velocity);

	vec3 curl = vec3(0.0);
	vec3 u = vec3(0.0);
	vec3 v = vec3(0.0);

	int thisBinId = GetBinIdFromPredictedPos(index);
	int binEmptySlot = FindNextEmptyNeighboringIndex(thisBinId);
	ParticleNeighbors neighbors = neighbor[thisBinId];

	for(int i = 0 ; i < binEmptySlot; ++i)
	{
		int neighborIndex = neighbors.neighborIndices[i];

		ParticleVec4 neighborvec4Val = particleInfoVec4[neighborIndex];

		ParticleVec4 val = particleInfoVec4[neighborIndex];
		u = vec3(val.velocity) - thisVel;
		v = grad_Wspiky(vec3(val.predictedPos) - thisPos, kernelRadius);

		curl += cross(u, v);
		
	}

	particleInfoVec4[index].force = vec4(curl, 1.0);
}

void ApplyVortAndViscXSPH()
{
	ParticleVec4 vec4Val = particleInfoVec4[index];
	ParticleVal valVal = particleInfoValue[index];

	vec3 thisPos = vec3(vec4Val.predictedPos);
	vec3 thisVel = vec3(vec4Val.velocity);
	vec3 curl = vec3(vec4Val.force);
	float density = valVal.density;

	vec3 n = vec3(0.0f);
	vec3 sumWeightNeighborVelocities = vec3(0.0);

	int thisBinId = GetBinIdFromPredictedPos(index);
	int binEmptySlot = FindNextEmptyNeighboringIndex(thisBinId);
	ParticleNeighbors neighbors = neighbor[thisBinId];

	for(int i = 0 ; i < binEmptySlot; ++i)
	{
		int neighborIndex = neighbors.neighborIndices[i];

		ParticleVec4 neighborvec4Val = particleInfoVec4[neighborIndex];
		
		ParticleVec4 Nvec4 = particleInfoVec4[neighborIndex];
		ParticleVal NVal = particleInfoValue[neighborIndex];

		n += (1 / max(NVal.density, 100.0f)) 
		* euclidean_distance(vec3(Nvec4.force)) 
		* grad_Wspiky(thisPos - vec3(Nvec4.predictedPos), kernelRadius);

		sumWeightNeighborVelocities += 
		(1 / max(NVal.density, 100.0f)) * 
		(thisVel - vec3(Nvec4.velocity)) * 
		Wpoly6(thisPos - vec3(Nvec4.predictedPos), kernelRadius);
		
	}

	vec3 nHat = vec3(0.0f);

	if(euclidean_distance2(n) > EPSILON)
	{
		nHat = n / length(n);
	}

	vec3 f_vc = fluidK_vc * cross(nHat, curl);

	particleInfoVec4[index].velocity = vec4(thisVel + 
										fluidC * sumWeightNeighborVelocities +
										fluidK_vc * tStep * f_vc, 1.0f);
}

void main()
{
	TimeStep();	
	ClipPredictedPosition();
	InsertParticles();

	for(int i = 0; i < fluidN; ++i)
	{
		CalcDensity();
		CalcLambda();
		CalcDeltaPiAndUpdate();
		ClipPredictedPosition();
	}
	ReCalcVelocities();
	CalcCurls();
	ApplyVortAndViscXSPH();

	particleInfoVec4[index].pos = particleInfoVec4[index].predictedPos;
	pos[index] =  particleInfoVec4[index].pos;


	for(int i = 0; i < neighborCount; ++i)
	{
		neighbor[index].neighborIndices[i] = -1;
	}
}