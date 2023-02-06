#version 420 core

in layout(location = 0) vec4 position;
in layout(location = 1) vec3 color;

uniform mat4 viewMat;
uniform mat4 projMat;

out VData{
	vec4 transformedColor;
}vColor;


void main(void)
{
	//position w is stands for particle speed
	vec4 viewPos = viewMat * vec4(position.xyz, 1.0);
	gl_Position = projMat * viewPos;

//	float speed = clamp(position.w, 0.0, 400.0);
//	vec3 largeSpeed = vec3(0.4, 0.4, 0.4);
//	vec3 smallSpeed = vec3(1.0, 1.0, 1.0);
//
//	vec3 clr = mix(smallSpeed, largeSpeed, vec3(speed / 200.0, speed / 200.0, speed / 200.0));
//	vColor.transformedColor = vec4(clr, (viewPos.z + 3000) / 3000);
	vColor.transformedColor = vec4(color, 1.0);
}
