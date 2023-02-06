#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

const int neighborCount = 100;
//const int pTotalCount = 30 * 30  * 30 ;

uniform int pTotalCount;

struct ParticleNeighbors
{
	int neighborIndices[neighborCount];
};

struct ParticleVec4
{
	vec4 velocity;
	//vec4 force;
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

layout(std430, binding = 3) buffer force
{
	vec4 particleForce[];
};

layout(std430, binding = 4) buffer particlePoses
{
	vec4 particlePos[];
};


const float PI = 3.1415926f;
//uniform float tStep;
const float tStep = 1.0f / 60;
const float pMoveSpeed = 1.0f / 61;
const float gravity = 9.8f;
const float h = 0.14f;
const float hsqr = h * h;
const float sigma = 0.1f;
const float wk = 315.f / (64 * PI * float(pow(h, 9)));
const float dwk = 15.f / (PI * float(pow(h, 6)));

const float scorrk = wk * float(pow(0.99 * hsqr, 3));

const float pDensity0 = 3000.f;
const float pRadius = 0.1f;
const float pMass = 1.25e-5f;

uniform float d;
uniform float wxMin;
uniform float wxMax;
uniform float wyMin;
uniform float wyMax;
uniform float wzMin;
uniform float wzMax;

//const float d = 1.5f;
//const float wxMin = -d;
//const float wxMax = 2.f * d;
//const float wyMin = -2.f * d;
//const float wyMax = d;
//const float wzMin = -d;
//const float wzMax = d;
//
const int xNum = int(ceil((wxMax - wxMin)/ h));
const int yNum = int(ceil((wyMax - wyMin) / h));
const int zNum = int(ceil((wzMax - wzMin) / h));

const int BubbleTypeNone = -1;
const int BubbleTypeSpray = 0;
const int BubbleTypeBubble = 1;
const int BubbleTypeFoam = 2;

const float minta=10;
const float maxta=45;
const float mink=0.1;
const float maxk=0.3;
const float kb=0.8;
const float kd=0.1;
const float mass=0.2;
const int kta=50;
const float maxlifetime=3;
const int LIMIT1 = 5;
const int LIMIT2 = 50;


const float kernelRadius = 0.14f;
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


float euclidean_distance2(const vec3 r) {
    return r.x * r.x + r.y * r.y + r.z * r.z;
}

float euclidean_distance(const vec3 r) {
    return sqrt(r.x * r.x + r.y * r.y + r.z * r.z);
}

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


float Clamp(float v, float minVal, float maxVal)
{
	if(v > maxVal)
		v = maxVal;

	if(v < minVal)
		v = minVal;

	return (v - minVal) / (maxVal - minVal);
}

float CalculateW(int i, int j)
{
	vec4 posI = particlePos[i];
	vec4 posJ = particlePos[j];

	float dx = posJ.x - posI.x;
	float dy = posJ.y - posI.y;
	float dz = posJ.z - posI.z;

//	float tmp = pRadius * pRadius - euclidean_distance2(vec3(dx, dy, dz));
//
//	if(tmp < EPSILON)
//		return 0.0f;
//
//	return wk * pow((tmp), 3);

	float rsqr = float(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

	if(hsqr < rsqr)
		return 0.0f;

	return wk * float(pow(hsqr - rsqr, 3));
}


vec3 CalculateDW(int i, int j)
{
	vec3 result = vec3(0.0);

	if(i == j)
		return result;

	vec4 posI = particlePos[i];
	vec4 posJ = particlePos[j];

	float dx = posJ.x - posI.x;
	float dy = posJ.y - posI.y;
	float dz = posJ.z - posI.z;

//	float radius2 = euclidean_distance2(vec3(dx, dy, dz));
//
//	if(radius2 >= pRadius * pRadius)
//		return vec3(0.0f);
//
//	if(radius2 < EPSILON)
//		return vec3(0.0f);
//
//	float radius = sqrt(radius2);
//
//	if(pRadius < radius)
//		return vec3(0.0f);
//
//	const float kernel_constant = - (15 / (PI * pow(pRadius, 6))) * 3 * pow(pRadius - radius, 2) / radius;
//
//	return vec3(kernel_constant * dx, 
//	kernel_constant * dy, 
//	kernel_constant * dz);



	float r = float(sqrt(float(pow(dx, 2) + pow(dy, 2) + pow(dz, 2))));

	if(h < r)
		return result;

	float c = 3 * dwk * float(pow(h - r, 2) / (r + 1e-10));

	result.x = dx * c;
	result.y = dy * c;
	result.z = dz * c;

	return result;
}

ivec3 Normalize(ivec3 val)
{
	ivec3 result = ivec3(0);
	int x = val.x;
	int y = val.y;
	int z = val.z;

	if(x < 0)
		x = 0;
	if(x > xNum)
	

	return result;
}
vec3 GetVelocity(float x, float y, float z)
{
	vec3 result = vec3(0);

	if(x < wxMin || x > wxMax || y < wyMin || y > wyMax || z < wzMin || z > wzMax)
		return result;

	int ix = int((x - wxMin) / h);
	int iy = int((y - wyMin) / h);
	int iz = int((z - wzMin) / h);

	int ixMin = int((x - h - wxMin) / h);
	int ixMax = int((x + h - wxMin) / h);
	int iyMin = int((y - h - wyMin) / h);
	int iyMax = int((y + h - wyMin) / h);
	int izMin = int((z - h - wzMin) / h);
	int izMax = int((z + h - wzMin) / h);
	ivec3 minVal = ivec3(ixMin, iyMin, izMin);
	ivec3 maxVal = ivec3(ixMax, iyMax, izMax);

	ivec3 normalizeMinval = Normalize(minVal);
	ivec3 normalizeMaxval = Normalize(maxVal);

	int newIxMin = normalizeMinval.x;
	int newIxMax = normalizeMaxval.x;

	int newIyMin = normalizeMinval.y;
	int newIyMax = normalizeMaxval.y;

	int newIzMin = normalizeMinval.z;
	int newIzMax = normalizeMaxval.z;

	float d = 0.0;
	for(int i = newIxMin; i < newIxMax + 1; ++i)
	{
		for(int j = newIyMin; j < newIyMax + 1; ++j)
		{
			for(int k = newIzMin; k < newIzMax + 1; ++k)
			{
				int index = i * newIxMax * newIyMax + j * newIyMax + k;
				//int index = i * 20 + j * 20 * 20 + k;
				vec4 posVal = particlePos[index];
				vec4 velVal = particleInfoVec4[index].velocity;

				float dx = posVal.x - x;
				float dy = posVal.y - y;
				float dz = posVal.z - z;

				float rsqr = pow(dx, 2) + pow(dy, 2) + pow(dz, 2);

				if(hsqr > rsqr)
				{
					float tmp = wk * pow(hsqr - rsqr, 3);
					result = result + vec3(velVal.x * tmp, velVal.y * tmp, velVal.z * tmp);
					d = d + wk * pow(hsqr - rsqr, 3);
				}
			}
		}
	}
	if(d == 0.0f)
		return vec3(0.0f);

	result /= d;

	return result;
}

int GetDensity(float x, float y, float z)
{
	int result = 0;

	if(x < wxMin || x > wxMax || y < wyMin || y > wyMax || z < wzMin || z > wzMax)
		return result;

	int ix = int((x - wxMin) / h);
	int iy = int((y - wyMin) / h);
	int iz = int((z - wzMin) / h);

	int ixMin = int((x - h - wxMin) / h);
	int ixMax = int((x + h - wxMin) / h);
	int iyMin = int((y - h - wyMin) / h);
	int iyMax = int((y + h - wyMin) / h);
	int izMin = int((z - h - wzMin) / h);
	int izMax = int((z + h - wzMin) / h);

	ivec3 minVal = ivec3(ixMin, iyMin, izMin);
	ivec3 maxVal = ivec3(ixMax, iyMax, izMax);

	ivec3 normalizeMinval = Normalize(minVal);
	ivec3 normalizeMaxval = Normalize(maxVal);

	int newIxMin = normalizeMinval.x;
	int newIxMax = normalizeMaxval.x;

	int newIyMin = normalizeMinval.y;
	int newIyMax = normalizeMaxval.y;

	int newIzMin = normalizeMinval.z;
	int newIzMax = normalizeMaxval.z;

	for(int i = newIxMin; i < newIxMax + 1; ++i)
	{
		for(int j = newIyMin; j < newIyMax + 1; ++j)
		{
			for(int k = newIzMin; k < newIzMax + 1; ++k)
			{
				//int index = i * 20 + j * 20 * 20 + k;
				int index = i * newIxMax * newIyMax + j * newIyMax + k;
				vec4 posVal = particlePos[index];

				float dx = posVal.x - x;
				float dy = posVal.y - y;
				float dz = posVal.z - z;

				float r = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

				if(h > r)
					result++;
			}
		}
	}
	return result;
}

vec3 GetNormal(int index)
{
	vec3 result = vec3(0.0);
	vec4 posVal = particlePos[index];

	int d = GetDensity(posVal.x, posVal.y, posVal.z);
	int xPlus = GetDensity(posVal.x + h, posVal.y, posVal.z);
	int yPlus = GetDensity(posVal.x, posVal.y + h, posVal.z);
	int zPlus = GetDensity(posVal.x, posVal.y, posVal.z + h);

	vec3 temp = vec3(xPlus - d, yPlus - d, zPlus - d);
	result = normalize(temp);
	return result;
}

float GetC(int i)
{
	//float densityVal = density[i];
	float densityVal = particleInfoValue[i].density;
	return densityVal / pDensity0 - 1;
}


void PredictPosition(uint index)
{
	//vec4 posVal = pos[index];
	vec4 posVal = particlePos[index];
	vec4 forceVal = particleForce[index];//vec4(particleForce[index], 1.f);
	vec4 velVal =  particleInfoVec4[index].velocity;
	vec4 predictedPosVal =  particleInfoVec4[index].predictedPos;

	velVal.x += forceVal.x * tStep;
	velVal.y += forceVal.y * tStep;
	velVal.z += forceVal.z * tStep;

	forceVal.x = 0.f;
	//forceVal.y = forceVal.y + (-gravity * tStep);
	forceVal.y = -gravity;
	forceVal.z = 0.f;

	posVal.x = predictedPosVal.x + velVal.x * tStep;
	posVal.y = predictedPosVal.y + velVal.y * tStep;
	posVal.z = predictedPosVal.z + velVal.z * tStep;

	particlePos[index]= posVal;
	//particleInfoVec4[index].predictedPos = posVal;
	particleForce[index] = forceVal;//vec3(forceVal);
}
void ComputeDensity()
{
	int index = int(gl_GlobalInvocationID.x);
	float densityVal = particleInfoValue[index].density;

	int neighborVal[neighborCount] = neighbor[index].neighborIndices;
	uint neighborIndex = 0;

	while(neighborVal[neighborIndex] != -1)
	{
		if(neighborIndex > neighborCount - 1)
		{
			break;
		}
		
		int neighboringIndex = neighborVal[neighborIndex];
		float w = CalculateW(index, neighboringIndex);
		
		densityVal += pMass * w;

		neighborIndex++;
	}
	particleInfoValue[index].density = densityVal;
}

void UpdateVelocityPos()
{
	int index = int(gl_GlobalInvocationID.x);
	vec4 posVal = particlePos[index];
	//vec4 velVal = velocity[index];
	vec4 predictedPosVal = particleInfoVec4[index].predictedPos;

	particleInfoVec4[index].velocity.x = (posVal.x - predictedPosVal.x) / pMoveSpeed;
	particleInfoVec4[index].velocity.y = (posVal.y - predictedPosVal.y) / pMoveSpeed;
	particleInfoVec4[index].velocity.z = (posVal.z - predictedPosVal.z) / pMoveSpeed;

	//particleForce[index] = particleInfoVec4[index].velocity;

	int neighborVal[neighborCount] = neighbor[index].neighborIndices;
	uint neighborIndex = 0;

	while(neighborVal[neighborIndex] != -1)
	{

		
		int neighboringIndex = neighborVal[neighborIndex];

		float w = CalculateW(index, neighboringIndex);
		
		vec4 neighborVel = particleInfoVec4[neighboringIndex].velocity;
		
		float vijx = neighborVel.x - particleInfoVec4[index].velocity.x;
		float vijy = neighborVel.y - particleInfoVec4[index].velocity.y;
		float vijz = neighborVel.z - particleInfoVec4[index].velocity.z;
		float c = 0.002f;

		particleInfoVec4[index].velocity.x += c * vijx * w;
		particleInfoVec4[index].velocity.y += c * vijy * w;
		particleInfoVec4[index].velocity.z += c * vijz * w;

		neighborIndex++;

		if(neighborIndex > neighborCount - 1)
		{
			break;
		}
	}
	predictedPosVal.x = posVal.x;
	predictedPosVal.y = posVal.y;
	predictedPosVal.z = posVal.z;

	//velocity[index] = velVal;
	particleInfoVec4[index].predictedPos = predictedPosVal;
}
void CollisionDetection()
{
	uint index = int(gl_GlobalInvocationID.x);
	vec4 posVal = particlePos[index];
	vec4 velVal = particleInfoVec4[index].velocity;
	float wallForce = -1.f;
	if(posVal.x < wxMin)
	{
		posVal.x = wxMin;
		velVal.x *= wallForce;
	}
	if(posVal.x > wxMax)
	{
		posVal.x = wxMax;
		velVal.x *= wallForce;
	}

	if(posVal.y < wyMin)
	{
		posVal.y = wyMin;
		velVal.y *= wallForce;
	}
	if(posVal.y > wyMax)
	{
		posVal.y = wyMax;
		velVal.y *= wallForce;
	}

	if(posVal.z < wzMin)
	{
		posVal.z = wzMin;
		velVal.z *= wallForce;
	}
	if(posVal.z > wzMax)
	{
		posVal.z = wzMax;
		velVal.z *= wallForce;
	}

	particlePos[index] = posVal;
	particleInfoVec4[index].velocity = velVal;
}

void ComputeLambda()
{
	int index = int(gl_GlobalInvocationID.x);
	float lambdaVal = particleInfoValue[index].lambda;
	int neighborVal[neighborCount] = neighbor[index].neighborIndices;
	uint neighborIndex = 0;
	float gradCSqr = 0;
	vec3 sum = vec3(0.0);

	while(neighborVal[neighborIndex] != -1)
	{
		if(neighborIndex > neighborCount - 1)
		{
			break;
		}
		
		int neighboringIndex = neighborVal[neighborIndex];

		vec3 t = CalculateDW(index, neighboringIndex);
		gradCSqr += float(pow(t.x, 2) + pow(t.y, 2) + pow(t.z, 2));
		sum.x += t.x;
		sum.y += t.y;
		sum.z += t.z;

		neighborIndex++;
	}
	gradCSqr += float(pow(sum.x, 2) + pow(sum.y, 2) + pow(sum.z, 2));
	gradCSqr *= float(pow(pMass / pDensity0, 2));
	lambdaVal = -GetC(index) / (gradCSqr + sigma);

	particleInfoValue[index].lambda = lambdaVal;
}
void ComputeDeltaP()
{
	int index = int(gl_GlobalInvocationID.x);
	float lambdaVal = particleInfoValue[index].lambda;
	int neighborVal[neighborCount] = neighbor[index].neighborIndices;
	uint neighborIndex = 0;
	vec3 posVal = vec3(particlePos[index]);
	vec3 sum = vec3(0.0);
	float s_corr = 0.0f;
	vec3 delta_pi = vec3(0.0);

	while(neighborVal[neighborIndex] != -1)
	{
		if(neighborIndex > neighborCount - 1)
		{
			break;
		}
		
		int neighboringIndex = neighborVal[neighborIndex];
		float neighborLambdaVal = particleInfoValue[neighboringIndex].lambda;
		vec3 neighborPosVal = vec3(particlePos[neighboringIndex]);

		float scorr = -0.1f * float(pow(CalculateW(index, neighboringIndex) / scorrk, 4));
		vec3 t = CalculateDW(neighboringIndex, index);
		float c = lambdaVal + neighborLambdaVal + scorr;

		t.x *= c;
		t.y *= c;
		t.z *= c;
		sum.x += t.x;
		sum.y += t.y;
		sum.z += t.z;

		neighborIndex++;
	}

	//delta_pi = delta_pi / pDensity0;

	float t = pMass / pDensity0;
	sum.x *= t;
	sum.y *= t;
	sum.z *= t;
	posVal.x += sum.x;
	posVal.y += sum.y;
	posVal.z += sum.z;

	particlePos[index] = vec4(posVal, 1.f);
	//particlePos[index] = vec4(posVal + clamp(delta_pi, -MAX_DELTA_PI, MAX_DELTA_PI), 1.f);
}


void main()
{
	uint gid = gl_GlobalInvocationID.x;
	
	PredictPosition(gid);
	ComputeDensity();

	for(int i = 0 ; i < 3; ++i)
	{
		ComputeLambda();
		ComputeDeltaP();
		CollisionDetection();
	}
	
	UpdateVelocityPos();

	for(int i = 0; i < neighborCount - 1; ++i)
	{
		neighbor[int(gid)].neighborIndices[i] = -1;
	}

	//GenerateBubble();
	//UpdateBubbles();
}