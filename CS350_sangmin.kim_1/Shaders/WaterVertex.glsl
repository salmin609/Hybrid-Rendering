#version 400 core

layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 to_ndc;
uniform mat4 cam;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCamera;
out vec3 fromLightVector;

const float tiling = 6.0;

void main(void)
{
	vec4 worldPosition = model * vec4(position.x, 0.0, position.y, 1.0);
	clipSpace = to_ndc * cam * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5) * tiling;
	toCamera = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}