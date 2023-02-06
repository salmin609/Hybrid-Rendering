#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
out vec4 vtxColor;

in VData
{
	vec4 transformedColor;
}vColor[];

void main(void)
{
	float size = 1.0f;
	for(int i = 0 ; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);
		vtxColor = vColor[i].transformedColor;
		EmitVertex();

		gl_Position = gl_in[i].gl_Position + vec4(0.0, size, 0.0, 0.0);
        vtxColor = vColor[i].transformedColor;
        EmitVertex();

        gl_Position = gl_in[i].gl_Position + vec4(size, 0.0, 0.0, 0.0);
        vtxColor = vColor[i].transformedColor;
        EmitVertex();

		gl_Position = gl_in[i].gl_Position + vec4(size, size, 0.0, 0.0);
        vtxColor = vColor[i].transformedColor;
        EmitVertex();

        EndPrimitive();
	}
}