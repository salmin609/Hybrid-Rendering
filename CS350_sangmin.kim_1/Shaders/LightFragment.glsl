#version 330 core

uniform vec3 color_val;

out vec4 frag_color;

void main()
{
   frag_color = vec4(color_val.x, color_val.y, color_val.z, 1);
};