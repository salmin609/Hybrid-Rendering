#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 offset;
layout(location = 4) in vec3 dir; 

out vec3 normal_vec;
out vec3 frag_pos;
out vec4 light;
out vec2 texture_coord;

//uniform mat4 to_ndc;
uniform mat4 model;
//uniform mat4 cam;
uniform vec4 light_pos;

layout(std140) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

void main()
{
	normal_vec = mat3(transpose(inverse(model))) * normal;
	light = light_pos;
	frag_pos = vec3(model * vec4(position + offset, 1));
	gl_Position = (to_ndc * cam * vec4(frag_pos, 1));
	texture_coord = texcoord;
};