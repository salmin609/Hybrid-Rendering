#version 410 core

uniform mat4 vertexTransform;

layout(location = 0) in vec3  vertex;

void main()
{
	gl_Position = vertexTransform * vec4( vertex, 1.0f );
}
