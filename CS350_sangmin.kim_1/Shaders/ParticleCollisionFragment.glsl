#version 420 core

in vec4 vtxColor;
out vec4 fragColor;

void main()
{
	fragColor = vtxColor;
}