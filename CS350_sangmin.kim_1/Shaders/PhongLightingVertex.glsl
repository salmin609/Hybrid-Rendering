#version 430 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexcoord;


struct Material {
    sampler2D diffuse;
    sampler2D specular;
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

vec3 frag_pos;
vec2 texVal;
out vec4 outColor;


uniform vec3 color_val;
uniform vec3 view_pos;
uniform Material material;
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

uniform int isCalcUVonGPU = 0;
uniform int texKind = 0;

layout(std140, binding = 0) uniform Matrices
{
	mat4 to_ndc;
	mat4 cam;
};

layout(std140, binding = 1) uniform Lights
{
    PointLight pointLights[NR_POINT_LIGHTS];

    DirLight dirLight[NR_POINT_LIGHTS];

    SpotLight spotLight[NR_POINT_LIGHTS];
};

vec3 getCentroidVector(vec3 vVertex, vec3 boundingBox0, vec3 boundingBox1)
{
    vec3 centroidVal = (boundingBox0 + boundingBox1) * 0.5f;
    return normalize(vVertex - centroidVal);
}

float degrees(float angleInRadians)
{
    const float PI = 3.14159265358979323846f;
    return angleInRadians * 180.0f / PI;
}

float radians(float angleInDegrees)
{
    const float PI = 3.14159265358979323846f;
    return angleInDegrees * (PI / 180.0f);
}



uniform mat4 model;


vec3 CalcDirLight(DirLight lightVal, vec3 normal, vec3 viewDir)
{
    vec3 lightDirVal = normalize(lightVal.position.xyz - frag_pos);

    vec3 lightDir = normalize(lightDirVal);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);

    vec3 norm = normalize(normal);
    vec3 lightHat = normalize(lightDir);
    vec3 reflectDir = (2 * (norm * lightHat)) * norm - lightHat;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = lightVal.ambient.xyz * ambientColor;
    vec3 diffuse = lightVal.diffuse.xyz * diff * vec3(texture(material.diffuse, texVal));
    vec3 specular = lightVal.specular.xyz * spec * vec3(texture(material.specular, texVal));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);

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
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(material.diffuse, texVal));
    vec3 specular = light.specular.xyz * spec * vec3(texture(material.specular, texVal));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position.xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);

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
    vec3 diffuse = light.diffuse.xyz * diff * vec3(texture(material.diffuse, texVal));
    vec3 specular = light.specular.xyz * spec * vec3(texture(material.specular, texVal));

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


vec2 calcCubeMap(vec3 vEntity)
{
    float x = vEntity.x;
    float y = vEntity.y;
    float z = vEntity.z;

    float absX = abs(x);
    float absY = abs(y);
    float absZ = abs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis = 0.f;
    float uc = 0.f;
    float vc = 0.f;
    vec2 uv;

    // POSITIVE X
    if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
    {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
    }

    // NEGATIVE X
    else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
    {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
    }

    // POSITIVE Y
    else if (bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
    }

    // NEGATIVE Y
    else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
    }

    // POSITIVE Z
    else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
    }

    // NEGATIVE Z
    else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
    }

    // Convert range from -1 to 1 to 0 to 1
    uv.x = 0.5f * (uc / maxAxis + 1.0f);
    uv.y = 0.5f * (vc / maxAxis + 1.0f);

    return uv;
}


void main()
{
	vec3 normal_vec = mat3(transpose(inverse(model))) * normal;
	frag_pos = vec3(model * vec4(position, 1));
    vec3 view_dir = normalize(view_pos - frag_pos);

    float viewFactor = length(frag_pos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    if (isCalcUVonGPU == 0)
    {
        texVal = aTexcoord;
    }
    else if (isCalcUVonGPU == 1)
    {
        //Calc
        vec3 boundingBox0 = vec3(-2.f, -2.f, -2.f);
        vec3 boundingBox1 = vec3(2.f, 2.f, 2.f);
        vec3 delta = vec3(1.f, 1.f, 1.f);

        vec3 v = position;
        vec2 uv = vec2(0.0f, 0.0f);
        vec3 normalVertex;

        normalVertex.x = (v.x - boundingBox0.x) / delta.x;
        normalVertex.y = (v.y - boundingBox0.y) / delta.y;
        normalVertex.z = (v.z - boundingBox0.z) / delta.z;

        vec3 centroidVec = getCentroidVector(v, boundingBox0, boundingBox1);

        float theta = 0.0f;
        float z = 0.0f;
        float phi = 0.0f;

        if (texKind == 0)
        {
            //Planar
            uv.x = (normalVertex.x - (-1.0f)) / (2.0f);
            uv.y = (normalVertex.y - (-1.0f)) / (2.0f);
        }
        else if (texKind == 1)
        {
            //Cylindrical
            theta = degrees(atan(centroidVec.y, centroidVec.x));
            theta += 180.0f;

            z = (centroidVec.z + 1.0f) * 0.5f;

            uv.x = theta / 360.f;
            uv.y = z;

        }
        else if (texKind == 2)
        {
            //sphereical
            theta = degrees((atan(centroidVec.y, centroidVec.x)));
            theta += 180.f;

            z = centroidVec.z;
            phi = degrees(acos(z / length(centroidVec)));

            uv.x = theta / 360.0f;
            uv.y = 1.0f - (phi / 180.0f);
        }
        else if (texKind == 3)
        {
            uv = calcCubeMap(centroidVec);
        }

        texVal = uv;
    }



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
    outColor = vec4(mix(fogColor, result, fogFactor), 1.0);


	gl_Position = to_ndc * cam * vec4(frag_pos, 1);
};