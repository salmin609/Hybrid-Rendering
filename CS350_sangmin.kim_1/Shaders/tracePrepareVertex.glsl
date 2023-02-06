#version 420 core

out VS_OUT
{
	vec3 rayOrigin;
	vec3 rayDirection;
}vs_out;

uniform vec3 rayOrigin;
uniform mat4 rayLookat;
uniform float aspect;

uniform vec3 directionScale = vec3(1.9, 1.9, 1.0);
uniform vec3 directionBias = vec3(0.0, 0.0, 0.0);

void main(void)
{
	vec4 vertices[4] = vec4[4](vec4(-1.0, -1.0, 1.0, 1.0),
                               vec4( 1.0, -1.0, 1.0, 1.0),
                               vec4(-1.0,  1.0, 1.0, 1.0),
                               vec4( 1.0,  1.0, 1.0, 1.0));

	vec4 pos = vertices[gl_VertexID];

	gl_Position = pos;
	vs_out.rayOrigin = rayOrigin * vec3(1.0, 1.0, -1.0);
	vs_out.rayDirection = (rayLookat * vec4(pos.xyz * directionScale * vec3(1.0, aspect, 2.0) + directionBias, 0.0)).xyz;
}