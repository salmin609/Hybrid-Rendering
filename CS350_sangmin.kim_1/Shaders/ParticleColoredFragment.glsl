#version 330 core
in vec3 colorVal;

out vec4 fragColor;

void main()
{
	fragColor = vec4(colorVal, 1);
}