#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 textureCoord;

out vec4 Vertex_UV;

void main()
{
	Vertex_UV = vec4(textureCoord, 0.0, 0.0);
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}
