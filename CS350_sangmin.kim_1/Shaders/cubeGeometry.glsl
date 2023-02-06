#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in VData
{
	vec4 passColor;
}vColor[];

out vec4 finalColor;

uniform mat4 mvp;

const float size = 1.f;

void CreateVertex(vec3 offset)
{
	vec4 actualOffset = vec4(offset * size, 0.0);
	vec4 worldPosition = gl_in[0].gl_Position + actualOffset;
	gl_Position = worldPosition;
	finalColor = vColor[0].passColor;
	EmitVertex();
}

void main(void)
{
	CreateVertex(vec3(-1.0, 1.0, 1.0));
	CreateVertex(vec3(-1.0, -1.0, 1.0));
	CreateVertex(vec3(1.0, 1.0, 1.0));
	CreateVertex(vec3(1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	CreateVertex(vec3(1.0, 1.0, 1.0));
	CreateVertex(vec3(1.0, -1.0, 1.0));
	CreateVertex(vec3(1.0, 1.0, -1.0));
	CreateVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	CreateVertex(vec3(1.0, 1.0, -1.0));
	CreateVertex(vec3(1.0, -1.0, -1.0));
	CreateVertex(vec3(-1.0, 1.0, -1.0));
	CreateVertex(vec3(-1.0, -1.0, -1.0));
	
	EndPrimitive();
	
	CreateVertex(vec3(-1.0, 1.0, -1.0));
	CreateVertex(vec3(-1.0, -1.0, -1.0));
	CreateVertex(vec3(-1.0, 1.0, 1.0));
	CreateVertex(vec3(-1.0, -1.0, 1.0));
	
	EndPrimitive();
	
	CreateVertex(vec3(1.0, 1.0, 1.0));
	CreateVertex(vec3(1.0, 1.0, -1.0));
	CreateVertex(vec3(-1.0, 1.0, 1.0));
	CreateVertex(vec3(-1.0, 1.0, -1.0));
	
	EndPrimitive();
	
	CreateVertex(vec3(-1.0, -1.0, 1.0));
	CreateVertex(vec3(-1.0, -1.0, -1.0));
	CreateVertex(vec3(1.0, -1.0, 1.0));
	CreateVertex(vec3(1.0, -1.0, -1.0));
	
	EndPrimitive();
}