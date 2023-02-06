#version 420 core

layout (location = 0) out vec4 color0;
layout (location = 1) out vec4 color1;

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	flat int materialIndex;
} fs_in;

uniform float bloomThreshMin = 1.0;
uniform float bloomThreshMax = 1.2;

struct materialT
{
	vec3 diffuseColor;
	vec3 specularColor;
	float specularPower;
	vec3 ambientColor;
};


layout (binding = 2, std140) uniform MATERIAL_BLOCK
{
	materialT material[1000];
}materials;

void main(void)
{
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);

	vec3 R = reflect(-L, N);

	materialT m = materials.material[fs_in.materialIndex];
	vec3 diffuse = max(dot(N, L), 0.0) * m.diffuseColor;
	vec3 specular = pow(max(dot(R, V), 0.0), m.specularPower) * m.specularColor;
	vec3 ambient = m.ambientColor;

	vec3 color = ambient + diffuse + specular;
	color0 = vec4(color, 1.0);

	float Y = dot(color, vec3(0.299, 0.587, 0.144));

	color = color * 4.0 * smoothstep(bloomThreshMin, bloomThreshMax, Y);
	color1 = vec4(color, 1.0);
}