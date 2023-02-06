#version 430 core

layout(location = 0) in vec2 position;

uniform mat4 VP;
uniform float pointSize = 1.5f;

void main()
{
	gl_PointSize = pointSize;
	gl_Position = VP * vec4(position, 0.0f, 1);
	//gl_Position = vec4(position, 0.0f, 1.f);
};