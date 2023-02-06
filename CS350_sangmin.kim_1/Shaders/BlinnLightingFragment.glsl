#version 430 core

layout(binding = 0) uniform sampler2D diffuse;
layout(binding = 1) uniform sampler2D specular;
struct Material {
    float shininess;
};

struct DirLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
struct SpotLight {
    vec4 position;
    vec4 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 cutoffs;
};
#define NR_POINT_LIGHTS 16  

in vec3 normal_vec;
in vec3 frag_pos;
in vec2 texVal;

out vec4 frag_color;

uniform int numOfDirlights;
uniform int numOfPointlights;
uniform int numOfSpotlights;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float zFar;
uniform float zNear;
uniform vec3 fogColor;
uniform vec3 ambientColor;

uniform vec3 color_val;
uniform vec3 view_pos;
uniform Material material;

layout(std140, binding = 1) uniform Lights
{
    PointLight pointLights[NR_POINT_LIGHTS];
    DirLight dirLight[NR_POINT_LIGHTS];
    SpotLight spotLight[NR_POINT_LIGHTS];
};

vec3 CalcDirLight(DirLight lightVal, vec3 normal, vec3 viewDir)
{
    vec3 lightDirVal = normalize(lightVal.position.xyz - frag_pos);
    
    //vec3 lightDir = normalize(lightDirVal);
    vec3 norm = normalize(normal);
    // diffuse shading
    float diff = max(dot(normal, lightDirVal), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDirVal + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);


    vec3 ambient = lightVal.ambient.xyz * ambientColor;
    vec3 diffuse = lightVal.diffuse.xyz * diff * vec3(texture(diffuse, texVal));
    vec3 specular = lightVal.specular.xyz * spec * vec3(texture(specular, texVal));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    vec3 norm = normalize(normal);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (constant + linear * distance +
        quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient.xyz * ambientColor;
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(diffuse, texVal));
    vec3 specular = light.specular.xyz * spec * vec3(texture(specular, texVal));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    vec3 norm = normalize(normal);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (constant + linear * distance +
        quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient.xyz * ambientColor;
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(diffuse, texVal));
    vec3 specular = light.specular.xyz * spec * vec3(texture(specular, texVal));

    float cutOff = light.cutoffs.x;
    float outerCutOff = light.cutoffs.y;

    float theta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon = (cutOff - outerCutOff);
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;


    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    float viewFactor = length(frag_pos);

    for (int i = 0; i < numOfDirlights; ++i)
    {
        result += CalcDirLight(dirLight[i], normal_vec, view_dir);
    }

    for (int i = 0; i < numOfPointlights; ++i)
    {
        result += CalcPointLight(pointLights[i], normal_vec, frag_pos, view_dir);
    }

    for (int i = 0; i < numOfSpotlights; ++i)
    {
        result += CalcSpotLight(spotLight[i], normal_vec, frag_pos, view_dir);
    }

    float fogFactor = (zFar - viewFactor) / (zFar - zNear);
    frag_color = vec4(mix(fogColor, result, fogFactor), 1.0);
};