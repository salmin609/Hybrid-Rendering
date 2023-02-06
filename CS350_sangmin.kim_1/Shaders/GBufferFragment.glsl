#version 430 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec3 depthValue;
layout(location = 4) out vec3 gUV;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 vTex;

//out vec4 color;

layout(binding = 0) uniform sampler2D texture_diffuse1;
layout(binding = 1) uniform sampler2D texture_specular1;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;

    gUV = vTex;
    float dist = length(FragPos);
    depthValue = vec3(dist);
}