#version 430 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec3 normal;

layout(location = 2)in vec3 birdPosition;
layout(location = 3)in vec3 birdVelocity;

out vec3 color;

uniform mat4 ndcMat;
uniform mat4 camMat;

mat4 MakeLookAt(vec3 forward, vec3 up)
{
	vec3 side = cross(forward, up);
	vec3 uFrame = cross(side, forward);

	return mat4(vec4(side, 0.0), vec4(uFrame, 0.0), vec4(forward, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
}

vec3 ChooseColor(float f)
{
	float r = sin(f * 6.2831853);
	float g = sin((f + 0.3333) * 6.2831853);
	float b = sin((f + 0.6666) * 6.2831853);

	return vec3(r,g,b) * 0.25 + vec3(0.75);
}

void main(void)
{
	mat4 lookAt = MakeLookAt(normalize(birdVelocity), vec3(0.0, 1.0, 0.0));
	vec4 objCoord = lookAt * vec4(position.xyz, 1.0);
	gl_Position = ndcMat * camMat * (objCoord + vec4(birdPosition, 0.0));

	vec3 N = mat3(lookAt) * normal;
	vec3 C = ChooseColor(fract(float(gl_InstanceID / float(1237.0))));

	color = mix(C * 0.2, C, smoothstep(0.0, 0.8, abs(N).z));
}
