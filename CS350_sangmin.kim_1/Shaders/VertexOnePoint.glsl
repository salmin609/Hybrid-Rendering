#version 430 core

uniform vec3 position;
uniform mat4 VP;

void main()
{
	gl_PointSize = 5.f;
	gl_Position = VP * vec4(position.xy, 0.0f, 1);
	//gl_Position = vec4(position.xy, 0.0, 1.0);
};