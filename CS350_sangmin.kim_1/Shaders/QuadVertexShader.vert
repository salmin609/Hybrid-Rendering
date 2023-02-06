#version 410 core

uniform mat4 model;
//uniform mat4 to_ndc;
//uniform mat4 cam;

layout(std140) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3  vPosition;
layout(location = 1) in vec3  vNormal;

uniform mat4 camMat;
uniform mat4 ndcMat;
uniform int inSetting = 1;

uniform vec3 diffuseColor = vec3( 1.f, 1.f, 1.f );

out VS_OUT
{
    vec3 fragDiffuse;
    vec3 fragNormal;

} vs_out;

void main()
{
    vs_out.fragDiffuse = diffuseColor;
    vs_out.fragNormal = vNormal;

    if(inSetting == 0)
	    gl_Position = to_ndc * cam * model * vec4( vPosition, 1.0f );

    else if(inSetting == 1)
    {
        gl_Position = ndcMat * camMat * model * vec4( vPosition, 1.0f );
    }
}
