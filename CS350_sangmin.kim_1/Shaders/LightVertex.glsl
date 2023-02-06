#version 330 core
layout(location = 0) in vec3 position;

//uniform mat4 to_ndc;
uniform mat4 model;
//uniform mat4 cam;
layout(std140) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

void main()
{
	gl_Position = to_ndc * cam * model * vec4(position, 1);
};