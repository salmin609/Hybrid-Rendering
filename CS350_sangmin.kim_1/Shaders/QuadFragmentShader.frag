#version 410 core

in VS_OUT
{
    vec3 fragDiffuse;
    vec3 fragNormal;

} fs_in;

uniform vec3 lightVector = vec3(0.0, 1.0, 1.0);
out vec4 fragColor;

void main()
{
    float N_dot_L = max( dot( normalize(fs_in.fragNormal), normalize(lightVector)), 0.0f );
	fragColor = vec4(fs_in.fragDiffuse * N_dot_L, 1);
	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
