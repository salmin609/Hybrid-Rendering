#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal_vec;
in vec3 frag_pos;
in vec4 light;
in vec2 texture_coord;

out vec4 frag_color;

uniform vec3 color_val;
uniform vec3 view_pos;
uniform Light light_;
uniform Material material;


vec3 Get_Color_Texture()
{
    vec3 ambient = light_.ambient * vec3(texture(material.diffuse, texture_coord));

    vec3 norm = normalize(normal_vec);
    vec3 lightDir = normalize(vec3(light.x, light.y, light.z) - frag_pos);

    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse_color = vec3(texture(material.diffuse, texture_coord)) * light_.diffuse;
    vec3 diffuse_color = vec3(texture(material.diffuse, texture_coord)) * diff * light_.diffuse;

    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-lightDir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, texture_coord)) * spec * light_.specular;

    vec3 result = (ambient + diffuse_color + specular);
    return result;
}

void main()
{
    vec3 result = Get_Color_Texture();
    frag_color = vec4(result , 1.0);
};