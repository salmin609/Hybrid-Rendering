/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Object.h
Purpose: Object class for handling objs.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#pragma once
#include "Mesh_Object.h"
#include "UniformManager.h"
#include "LightInfo.h"

class Buffer;
class ObjLoad;
class Object_ExceptShadow;

struct Material_
{
    Vector ambient;
    Vector diffuse;
    Vector specular;
    float shiness;
};

class Object
{
public:
    Object();
    Object(Mesh_Object* mesh_val, Point pos, Material_* mat, Mesh* shadow_mesh = nullptr);
    Object(Mesh* mesh_val, Point pos, Material_* mat, Vector color, Shader* shaderVal);
    Object(std::string objFilePath, Material_* mat, Shader* shaderVal, Mesh::UVType uvType = Mesh::UVType::CUBE_MAPPED_UV);
    Object(Mesh* meshVal);
    ~Object();

	void Init();
    void Draw();
    void DrawByElement(Matrix& ndc, Matrix& camMat);
    void End_Draw();
    void Select_Mesh();
    void Send_Uniform(Matrix world_mat);
    void Set_Light_Pos(Point light);
    void Set_Second_Light_Pos(Point light);
    void Set_Shadow(Mesh* mesh_val);
    void Set_Rot_Rate(float val);
    void Set_Rot_Axis(Vector axis);
    void Set_Camera_Pos(Point cam_pos);
    Matrix Get_Model_To_World();
    Matrix GetModelToWorldOutlineScaling(float increment);
    Mesh* Get_Mesh();
    Mesh* GetNormalDisplayingMesh();
    void SetPosition(Point newPos);
    Point& Get_Obj_Pos();
    Vector Get_Scale();
    Vector& Get_ScaleRef();
    void Set_Scale(Vector scale);
    void Set_Scale(float scale);
    void Set_T(float t_);
    float Get_T();
    void Set_Selected(bool toggle);
    bool Get_Selected();
    float& Get_Rot_Rate();
    Vector& Get_Rot_Axis();
    void Set_Material(Material_* mat_);
    Object_ExceptShadow* Get_Shadow();
    void SetVAO();
    void Initialize_Uniform();
    void IncreScale();
    bool IsElemented();
    void SetPlane(const Hcoord& planeVec);
    void InitializeSphereMesh(Shader* shaderVal);
    Mesh* CreateSphere(float radius, int sectorCount, int stackCount);
    void SetShader(Shader* shader);
    void SetNormalDisplayBuffer(Shader* normalPlayShader);
    void BindNormalDisplayVAO();
    void BindFaceNormalDisplayVAO();
    bool IsNormalVAOExist();
    bool IsFaceNormalVAOExist();
    void ReInitUniforms(const int& shaderId);
    Buffer* normalDisplayBuffer = nullptr;
    int orbitIndex = 0;
    int orbitRotatingSpeed = 8;
    bool isLightObj = false;
    LightKinds lightKind = LightKinds::PointLight;
    Light lightProperty{
        Vector4(0.f),
        Vector4(.5f),
        Vector4(.5f),
        Vector4(.5f)
    };
    Material_* obj_mat = nullptr;
    int textureSlotOffsetInShader = 0;
    Vector4 emissive = Vector4(0.f);
    Vector2 cutoffs = { cos(radians(12.5f)) ,
    	cos(radians(17.5f)) };
    
private:
    struct LightInfo
    { 
        Point lightPos;
        Vector diffuse{0.5f, 0.5f, 0.5f};
        Vector ambient{0.2f, 0.2f, 0.2f};
        Vector specular{10.f, 10.f, 10.f};
    };
	struct ObjInfo
	{
        Vector scale{ 1.f, 1.f, 1.f };
        Point pos;
        Vector color;
        Vector rotAxis{ 0.f, 0.f, 1.f };
        float rotRate = 0.f;
	};
    Hcoord planeVector;
	
    Mesh* mesh;
    Mesh* normalMesh;
    VAO* normalPlayVao = nullptr;
    VAO* faceNormalPlayVao = nullptr;

    Object_ExceptShadow* shadow = nullptr;
    Point camera_pos;
    UniformManager uniforms;
	
    unsigned int shader_id;
    unsigned int vao_id;
	
    float t;
    bool selected = false;


    //Vector color;
    ObjInfo objInfo;
    LightInfo lightInfo;
    //Light* lights;

    ObjLoad* objLoad;

};