#pragma once
#include "Affine.h"
enum class LightKinds {
    DirectionalLight = 0,
    PointLight,
    SpotLight
};
struct Light 
{
    Vector4 position;
    Vector4 ambient;
    Vector4 diffuse;
    Vector4 specular;
};

struct PointLight : public Light
{
    /*Vector3 position; float p1;
    Vector3 ambient; float p2;
    Vector3 diffuse; float p3;
    Vector3 specular; float p4;*/
};

struct DirLight : public Light
{
};

struct SpotLight
{
    /*Vector3 position; float p1 = 0.f;
    Vector3 direction; float p2 = 0.f;
    Vector3 ambient; float p3 = 0.f;
    Vector3 diffuse; float p4 = 0.f;
    Vector3 specular; float p5 = 0.f;
    float cutOff; Vector3 vec1 = Vector3{0.0f, 0.0f, 0.0f};
    float outerCutOff; Vector3 vec2 = Vector3{ 0.0f, 0.0f, 0.0f };*/

    Vector4 position;
    Vector4 direction;
    Vector4 ambient;
    Vector4 diffuse;
    Vector4 specular;
    Vector4 cutoffs;
};