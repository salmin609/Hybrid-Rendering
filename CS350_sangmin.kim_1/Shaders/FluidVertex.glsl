#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 c;
//layout(location = 2) in float radius;

uniform mat4 MVP;
uniform mat4 MV;
out vec3 posEye;
out float pointRadius;
out vec3 posWorld;
uniform float radius;
//out VData{
//	vec3 color;
//}vColor;

out vec3 col;

void main()
{
	posEye = vec3(MV * vec4(vertexPos, 1.f));
	posWorld = vec3(MVP * vec4(vertexPos, 1));
	gl_Position = MVP * vec4(vertexPos, 1);

	col = c;
	float dist = length(posEye);
	float pointScale = 1.f;
	gl_PointSize = (radius * 1000) * (pointScale  / dist);
	pointRadius = gl_PointSize;
}