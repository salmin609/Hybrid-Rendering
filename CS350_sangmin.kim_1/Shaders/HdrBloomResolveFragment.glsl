#version 430 core

layout (binding = 0) uniform sampler2D hdrImage;
layout (binding = 1) uniform sampler2D bloomImage;

uniform float exposure = 1.5;
uniform float bloomFactor = 0.8;
uniform float sceneFactor = 1.0;

out vec4 color;

void main(void)
{
	vec4 c = vec4(0.0);

	c += texelFetch(hdrImage, ivec2(gl_FragCoord.xy), 0) * sceneFactor;
	c += texelFetch(bloomImage, ivec2(gl_FragCoord.xy), 0) * bloomFactor;

	c.rgb = vec3(1.0) - exp(-c.rgb * exposure);
	color = c;
}