#version 420 core

in VS_OUT
{
	vec3 rayOrigin;
	vec3 rayDirection;
}fs_in;

layout(location = 0) out vec3 color;
layout(location = 1) out vec3 origin;
layout(location = 2) out vec3 reflected;
layout(location = 3) out vec3 refracted;
layout(location = 4) out vec3 reflectedColor;
layout(location = 5) out vec3 refractedColor;

void main(void)
{
	color = vec3(0.0);
	origin = fs_in.rayOrigin;
	reflected = fs_in.rayDirection;
	refracted = vec3(0.0);
	reflectedColor = vec3(1.0);
	refractedColor = vec3(0.0);
}