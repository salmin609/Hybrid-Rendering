#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	flat int materialIndex;
} vs_out;

uniform vec3 lightPos = vec3(0.0, 0.0, 0.0);

layout (binding = 1, std140) uniform TRANSFORM_BLOCK
{
	mat4 matProj;
	mat4 matView;
	mat4 matModel[1000];
}transforms;

void main(void)
{
	mat4 matMv = transforms.matProj * transforms.matView * transforms.matModel[gl_InstanceID];

	vec4 p = matMv * vec4(position, 1.0);

	vs_out.N = mat3(matMv) * normal;

	vs_out.L = lightPos - p.xyz;

	vs_out.V = -p.xyz;

	vs_out.materialIndex = gl_InstanceID;

	//gl_Position = transforms.matProj * p;
	gl_Position = p;
}