#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexcoord;

out vec3 normal_vec;
out vec3 frag_pos;
out vec2 texVal;

layout(std140, binding = 0) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

uniform mat4 model;
uniform int isCalcUVonGPU = 0;
uniform int texKind = 0;

vec3 getCentroidVector(vec3 vVertex, vec3 boundingBox0, vec3 boundingBox1)
{
	vec3 centroidVal = (boundingBox0 + boundingBox1) * 0.5f;
	return normalize(vVertex - centroidVal);
}

float degrees(float angleInRadians)
{
	const float PI = 3.14159265358979323846f;
	return angleInRadians * 180.0f / PI;
}

float radians(float angleInDegrees)
{
	const float PI = 3.14159265358979323846f;
	return angleInDegrees * (PI / 180.0f);
}


vec2 calcCubeMap(vec3 vEntity)
{
	float x = vEntity.x;
	float y = vEntity.y;
	float z = vEntity.z;

	float absX = abs(x);
	float absY = abs(y);
	float absZ = abs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	float maxAxis = 0.f;
	float uc = 0.f;
	float vc = 0.f;
	vec2 uv;

	// POSITIVE X
	if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
	{
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -z;
		vc = y;
	}

	// NEGATIVE X
	else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
	{
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = z;
		vc = y;
	}

	// POSITIVE Y
	else if (bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = x;
		vc = -z;
	}

	// NEGATIVE Y
	else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = x;
		vc = z;
	}

	// POSITIVE Z
	else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = x;
		vc = y;
	}

	// NEGATIVE Z
	else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -x;
		vc = y;
	}

	// Convert range from -1 to 1 to 0 to 1
	uv.x = 0.5f * (uc / maxAxis + 1.0f);
	uv.y = 0.5f * (vc / maxAxis + 1.0f);

	return uv;
}


void main()
{
	normal_vec = mat3(transpose(inverse(model))) * normal;
	frag_pos = vec3(model * vec4(position, 1));

	if (isCalcUVonGPU == 0)
	{
		texVal = aTexcoord;
	}
	else if (isCalcUVonGPU == 1)
	{
		//Calc
		vec3 boundingBox0 = vec3(-2.f, -2.f, -2.f);
		vec3 boundingBox1 = vec3(2.f, 2.f, 2.f);
		vec3 delta = vec3(1.f, 1.f, 1.f);

		vec3 v = position;
		vec2 uv = vec2(0.0f, 0.0f);
		vec3 normalVertex;

		normalVertex.x = (v.x - boundingBox0.x) / delta.x;
		normalVertex.y = (v.y - boundingBox0.y) / delta.y;
		normalVertex.z = (v.z - boundingBox0.z) / delta.z;

		vec3 centroidVec = getCentroidVector(v, boundingBox0, boundingBox1);

		float theta = 0.0f;
		float z = 0.0f;
		float phi = 0.0f;

		if (texKind == 0)
		{
			//Planar
			uv.x = (normalVertex.x - (-1.0f)) / (2.0f);
			uv.y = (normalVertex.y - (-1.0f)) / (2.0f);
		}
		else if (texKind == 1)
		{
			//Cylindrical
			theta = degrees(atan(centroidVec.y, centroidVec.x));
			theta += 180.0f;

			z = (centroidVec.z + 1.0f) * 0.5f;

			uv.x = theta / 360.f;
			uv.y = z;

		}
		else if (texKind == 2)
		{
			//sphereical
			theta = degrees((atan(centroidVec.y, centroidVec.x)));
			theta += 180.f;

			z = centroidVec.z;
			phi = degrees(acos(z / length(centroidVec)));

			uv.x = theta / 360.0f;
			uv.y = 1.0f - (phi / 180.0f);
		}
		else if (texKind == 3)
		{
			uv = calcCubeMap(centroidVec);
		}

		texVal = uv;
	}

	gl_Position = to_ndc * cam * vec4(frag_pos, 1);

};