#version 410 core

out vec4 fragColor;

uniform vec3 color = vec3(1.0, 0.0, 0.0);

void main()
{
	fragColor = vec4(color, 1.0);
}
