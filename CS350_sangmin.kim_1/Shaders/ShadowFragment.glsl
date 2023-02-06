#version 330 core

uniform vec3 color_val;
uniform vec3 light_pos;

out vec4 frag_color;
in vec3 frag_pos;

void main()
{
   float distance = distance(normalize(light_pos), normalize(frag_pos));

   float val = min(distance, 0.9);

   frag_color = vec4(color_val.x, color_val.y, color_val.z, 1.f - val);
};