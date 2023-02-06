#version 430 core

layout(binding = 0) uniform sampler2D diffuse;
layout(binding = 1) uniform sampler2D specular;
struct Material {
    vec4 emissive;
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

uniform vec3 color_val;
uniform vec3 view_pos;
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
uniform int reflectOnly;
uniform int refractOnly;
uniform float fersnelFactor;
layout(binding = 2) uniform samplerCube skybox;

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

    vec3 lightDir = normalize(lightDirVal);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 norm = normalize(normal);
    vec3 lightHat = normalize(lightDir);
    vec3 reflectDir = (2 * (norm * lightHat)) * norm - lightHat;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = lightVal.ambient.xyz * ambientColor;
    vec3 diffuse = lightVal.diffuse.xyz * diff * vec3(texture(diffuse, texVal));
    vec3 specular = lightVal.specular.xyz * spec * vec3(texture(specular, texVal));
    return material.emissive.xyz + (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 norm = normalize(normal);
    vec3 lightHat = normalize(lightDir);
    vec3 reflectDir = (2 * (norm * lightHat)) * norm - lightHat;

    //spec
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
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
    return material.emissive.xyz + (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 norm = normalize(normal);
    vec3 lightHat = normalize(lightDir);
    vec3 reflectDir = (2 * (norm * lightHat)) * norm - lightHat;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
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
    return material.emissive.xyz + (ambient + diffuse + specular);
}

vec3 myReflect(vec3 I, vec3 N)
{
    vec3 reflect;
    reflect = I - 2.0 * dot(N, I) * N;
    return reflect;
}
vec3 myRefract(vec3 I, vec3 N, float eta)
{
    float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
    vec3 refract;
    if(k >= 0.0)
        refract = eta * I - (eta * dot(N, I) + sqrt(k)) * N;
    else
        return vec3(0.0, 0.0, 0.0);
       
    return refract;
}

float fresnel(vec3 I, vec3 N, float ior)
{
    float dotVal = dot(I, N);
    float cosi = clamp(-1, 1, dotVal);
    float etai = 1;
    float etat = ior;
    if (cosi > 0) 
    {
        //std::swap(etai, etat); 
        float temp = etai;
        etai = etat;
        etat = temp;
    }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrt(max(0.f, 1 - cosi * cosi));
    float kr = 0.f;
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrt(max(0.f, 1 - sint * sint));
        cosi = abs(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }

    return kr;
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

void main()
{
    vec3 viewVec = view_pos - frag_pos;
    float viewFactor = length(frag_pos);

    vec3 view_dir = normalize(viewVec);
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

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
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(frag_pos - view_pos);
    vec3 R = vec3(0.0, 0.0, 0.0);
    float fresnelval = fresnel(I, normal_vec, fersnelFactor);
    if (reflectOnly == 1)
    {
        R += fresnelval * myReflect(I, normalize(normal_vec));
    }
    else if (reflectOnly == 1)
    {
        R += fresnelval * myRefract(I, normalize(normal_vec), ratio);
    }
    else
    {
        R += fresnelval * myReflect(I, normalize(normal_vec));
        R += fresnelval * myRefract(I, normalize(normal_vec), ratio);
    }


    frag_color += vec4(texture(skybox, R).rgb, 1.0);
};