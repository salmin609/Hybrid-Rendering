#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 dir; 
layout(location = 3) in vec3 rotation; 


mat4 rotationMatrix(vec3 axis, float angle) 
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 rotate(vec3 v, vec3 axis, float angle) 
{
	mat4 m = rotationMatrix(axis, angle);
	return (m * vec4(v, 1.0)).xyz;
}

out vec3 colorVal;

layout(std140) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

//uniform mat4 to_ndc;
uniform mat4 model;
//uniform mat4 cam;

void main()
{
    mat4 rotateMat = rotationMatrix(rotation, 10.0);
	gl_Position = to_ndc * cam * model * rotateMat * vec4(position + dir, 1);
	
	colorVal = color;
};