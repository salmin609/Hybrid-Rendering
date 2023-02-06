#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gUV;
uniform sampler2D gDepth;

struct PointLight {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};


const int NR_LIGHTS = 32;
uniform vec3 viewPos;
uniform int stateType = 0;
uniform float constant = 1.0;
uniform float linear = 0.7;
uniform float quadratic = 1.8;

layout(std140, binding = 0) uniform Lights
{
	PointLight pointLights[NR_LIGHTS];
};

void main()
{
	// retrieve data from gbuffer
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 DiffuseTexture = texture(gAlbedoSpec, TexCoords).rgb;
	float SpecularTexture = texture(gAlbedoSpec, TexCoords).a;
	vec3 uvs = texture(gUV, TexCoords).rgb;

	float depthFromEye = texture(gDepth, TexCoords).r;
	// then calculate lighting as usual
	vec3 lighting = DiffuseTexture * 0.1; // hard-coded ambient component
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir;
	for (int i = 0; i < NR_LIGHTS; ++i)
	{
		// calculate distance between light source and current fragment
		float distance = length(pointLights[i].position.xyz - FragPos);
		// diffuse
		vec3 lightDir = normalize(pointLights[i].position.xyz - FragPos);
		vec3 diff = max(dot(Normal, lightDir), 0.0) * DiffuseTexture;

		vec3 norm = normalize(Normal);
		vec3 lightHat = normalize(lightDir);
		reflectDir = (2 * (norm * lightHat)) * norm - lightHat;

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
		float attenuation = 1.0 / (constant + linear + distance + quadratic * (distance * distance));
		vec3 ambient = pointLights[i].ambient.xyz;
		vec3 diffuse = pointLights[i].diffuse.xyz * diff;
		vec3 specular = pointLights[i].specular.xyz * spec;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		lighting += (ambient + diffuse + specular);
	}

	if (stateType == 0)
	{
		FragColor = vec4(lighting, 1.0);
	}
	else if (stateType == 1)
	{
		FragColor = vec4(FragPos, 1.0);
	}
	else if (stateType == 2)
	{
		FragColor = vec4(Normal, 1.0);
	}
	else if (stateType == 3)
	{
		FragColor = vec4(uvs, 1.0);
	}
	else if (stateType == 4)
	{
		FragColor = vec4(depthFromEye, depthFromEye, depthFromEye, 1.0);
	}
}