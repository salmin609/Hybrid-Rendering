/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Object.cpp
Purpose: Object class for handling objs.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#include "Object.h"
#include "Object_ExceptShadow.h"
#include "Projection.h"
#include "ObjLoad.h"
#include "Buffer.hpp"
#include "VAO.h"
#include "Graphic.h"
Object::Object()
{
	
}

Object::Object(Mesh_Object* mesh_val, Point pos, Material_* mat, Mesh* shadow_mesh) : shadow(nullptr), obj_mat(mat), t(0.f)
{
	objInfo.pos = pos;
	
	mesh = mesh_val;
	if (shadow_mesh != nullptr)
	{
		Set_Shadow(shadow_mesh);
	}
	Init();
}

Object::Object(Mesh* mesh_val, Point pos, Material_* mat, Vector color, Shader* shaderVal) : shadow(nullptr), obj_mat(mat), t(0.f)
{
	objInfo.pos = pos;
	objInfo.color = color;
	mesh = mesh_val;
	mesh->SetShader(shaderVal);

	Init();
}

Object::Object(std::string objFilePath, Material_* mat, Shader* shaderVal, Mesh::UVType uvType)
{
	objLoad = new ObjLoad();
	mesh = new Mesh();
	mesh->SetShader(shaderVal);
	//SetShader(shaderVal);

	obj_mat = mat;
	objInfo.pos = Point{ 0.f, 0.f, 0.f };
	objInfo.color = Vector{ 1.f, 1.f, 1.f };

	objLoad->ReadObjFile(objFilePath, mesh);

	if(!Graphic::instance->isCalcUVonGPU)
	{
		mesh->calcUVs(uvType);
	}
	mesh->Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());
	Init();
}

Object::Object(Mesh* meshVal)
{
	mesh = meshVal;
	mesh->Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());
	Init();
}

Object::~Object()
{
	if (shadow != nullptr)
	{
		delete shadow;
	}
}

void Object::Init()
{
	shader_id = mesh->Get_Shader_Id();
	vao_id = mesh->Get_VAO_Id();
	uniforms.SetShaderId(shader_id);
	Initialize_Uniform();
	mesh->Unbind();
}

void Object::InitializeSphereMesh(Shader* shaderVal)
{
	mesh = CreateSphere(.1f, 36, 18);
	mesh->SetShader(shaderVal);
	mesh->Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());

	objInfo.pos = Point{ 0.f, 0.f, 0.f };
	objInfo.color = Vector{ 1.f, 1.f, 1.f };

	shadow = nullptr;
	Init();
}

void Object::Draw()
{
	Select_Mesh();
	//Send_Uniform(Get_Model_To_World());

	if (mesh->IsElemented())
	{
		glDrawElements(GL_TRIANGLES, mesh->FaceCount(), GL_UNSIGNED_INT, nullptr);
	}
	else if(mesh->IsInstancing())
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->FaceCount() * 3, 
			mesh->InstancingNum());
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mesh->FaceCount() * 3);
	}
	 
	End_Draw();
}



void Object::End_Draw()
{
	mesh->Unbind();
}

void Object::Select_Mesh()
{
	shader_id = mesh->Get_Shader_Id();
	vao_id = mesh->Get_VAO_Id();

	glUseProgram(shader_id);
	glBindVertexArray(vao_id);
	mesh->SetTexture(textureSlotOffsetInShader);

	if (mesh->IsElemented())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetElementId());
	}
	//Initialize_Uniform();

}

void Object::Send_Uniform(Matrix world_mat)
{
	uniforms("model", &world_mat);
	uniforms("view_pos", &camera_pos);
	uniforms("plane", &planeVector);
	
	if (obj_mat != nullptr)
	{
		uniforms("material.ambient", &obj_mat->ambient);
		uniforms("material.diffuse", &obj_mat->diffuse);
		uniforms("material.specular", &obj_mat->specular);
		uniforms("material.shininess", &obj_mat->shiness);
	}
	uniforms("material.emissive", &emissive);

	uniforms("light_.ambient", &lightInfo.ambient);
	uniforms("light_.diffuse", &lightInfo.diffuse);
	uniforms("light_.specular", &lightInfo.specular);
	uniforms("light_.lightPos", &lightInfo.lightPos);

 	uniforms("dirLight.ambient", &lightInfo.ambient);
	uniforms("dirLight.diffuse", &lightInfo.diffuse);
	uniforms("dirLight.position", &lightInfo.lightPos);
	uniforms("dirLight.specular", &lightInfo.specular);

	std::vector<Object*> objs = Graphic::instance->objects;

	const size_t objsSize = objs.size();
	int pointLightindexInShader = 0;
	int dirLightindexInShader = 0;
	int spotLightindexInShader = 0;

	for (size_t i = 0; i < objsSize; ++i)
	{
		const Object* obj = objs[i];

		if (obj->isLightObj)
		{
			if (obj->lightKind == LightKinds::DirectionalLight)
			{
				dirLightindexInShader++;
			}
			else if (obj->lightKind == LightKinds::PointLight)
			{
				pointLightindexInShader++;
			}
			else if (obj->lightKind == LightKinds::SpotLight)
			{
				spotLightindexInShader++;
			}
		}

	}

	dirLightindexInShader = std::min(dirLightindexInShader, 16);
	pointLightindexInShader = std::min(pointLightindexInShader, 16);
	spotLightindexInShader = std::min(spotLightindexInShader, 16);

	uniforms("numOfDirlights", &dirLightindexInShader);
	uniforms("numOfPointlights", &pointLightindexInShader);
	uniforms("numOfSpotlights", &spotLightindexInShader);
	
	uniforms("constant", &Graphic::instance->globalAtts.y);
	uniforms("linear", &Graphic::instance->globalAtts.x);
	uniforms("quadratic", &Graphic::instance->globalAtts.z);
	uniforms("texKind", &Graphic::instance->texType);
	uniforms("diffuseColor", &lightProperty.diffuse);
	uniforms("fogColor", &Graphic::instance->airColor);

	uniforms("ambientColor", &Graphic::instance->ambientColor);
	

	if (Graphic::instance->isCalcUVonGPU)
	{
		int value = 1;
		uniforms("isCalcUVonGPU", &value);
	}
	else
	{
		int value = 0;
		uniforms("isCalcUVonGPU", &value);
	}

	if(Graphic::instance->isReflectOnly)
	{
		int value = 1;
		uniforms("reflectOnly", &value);
	}
	else
	{
		int value = 0;
		uniforms("reflectOnly", &value);
	}
	if (Graphic::instance->isRefractOnly)
	{
		int value = 1;
		uniforms("refractOnly", &value);
	}
	else
	{
		int value = 0;
		uniforms("refractOnly", &value);
	}

	uniforms("zNear", &Graphic::instance->zNear);
	uniforms("zFar", &Graphic::instance->zFar);

	uniforms("fersnelFactor", &Graphic::instance->fersnelVal);

	uniforms("color_val", &objInfo.color);

	int temp = 0;
	uniforms("differentCamMatrix", &temp);
	uniforms("inSetting", &temp);
}

void Object::Set_Light_Pos(Point light)
{
	lightInfo.lightPos = light;
}

void Object::Set_Second_Light_Pos(Point light)
{
	(light);
	//second_light_pos = light;
}

void Object::Set_Shadow(Mesh* mesh_val)
{
	shadow = new Object_ExceptShadow(mesh_val);
	shadow->Set_Color(Vector(0, 0, 0));
}

void Object::Set_Rot_Rate(float val)
{
	objInfo.rotRate = val;
}

void Object::Set_Rot_Axis(Vector axis)
{
	objInfo.rotAxis = axis;
}

void Object::Set_Camera_Pos(Point cam_pos)
{
	camera_pos = cam_pos;
}

Matrix Object::Get_Model_To_World()
{
	return translate(objInfo.pos - Point(0, 0, 0)) * rotate(objInfo.rotRate, objInfo.rotAxis) * scale(objInfo.scale.x, objInfo.scale.y, objInfo.scale.z);
}

Matrix Object::GetModelToWorldOutlineScaling(float increment)
{
	return translate(objInfo.pos - Point(0, 0, 0)) * rotate(objInfo.rotRate, objInfo.rotAxis) * scale(objInfo.scale.x + increment, objInfo.scale.y + increment, objInfo.scale.z + increment);
}

Mesh* Object::Get_Mesh()
{
	return mesh;
}

Mesh* Object::GetNormalDisplayingMesh()
{
	return normalMesh;
}

void Object::SetPosition(Point newPos)
{
	//objInfo.pos = newPos;
	objInfo.pos.x = newPos.x;
	objInfo.pos.y = newPos.y;
	objInfo.pos.z = newPos.z;
	objInfo.pos.w = 1.f;
}

Point& Object::Get_Obj_Pos()
{
	return objInfo.pos;
}

Vector Object::Get_Scale()
{
	return objInfo.scale;
}

Vector& Object::Get_ScaleRef()
{
	return objInfo.scale;
}

void Object::Set_Scale(Vector scale)
{
	this->objInfo.scale = scale;
}

void Object::Set_Scale(float scale)
{
	this->objInfo.scale = Vector{ scale, scale, scale };
}

void Object::Set_T(float t_)
{
	t = t_;
}

float Object::Get_T()
{
	return t;
}

void Object::Set_Selected(bool toggle)
{
	selected = toggle;
}

bool Object::Get_Selected()
{
	return selected;
}

float& Object::Get_Rot_Rate()
{
	return objInfo.rotRate;
}

Vector& Object::Get_Rot_Axis()
{
	return objInfo.rotAxis;
}

void Object::Set_Material(Material_* mat_)
{
	obj_mat = mat_;
}

Object_ExceptShadow* Object::Get_Shadow()
{
	return shadow;
}

void Object::SetVAO()
{
	glBindVertexArray(mesh->Get_VAO_Id());
}


void Object::Initialize_Uniform()
{
	uniforms.Clear();
	uniforms.AddUniform("to_ndc", UniformManager::Matrix4x4);
	uniforms.AddUniform("second_light_pos", UniformManager::Vector3);
	uniforms.AddUniform("model", UniformManager::Matrix4x4);
	uniforms.AddUniform("cam", UniformManager::Matrix4x4);
	uniforms.AddUniform("view_pos", UniformManager::Vector3);
	uniforms.AddUniform("material.ambient", UniformManager::Vector3);
	uniforms.AddUniform("material.diffuse", UniformManager::Vector3);
	uniforms.AddUniform("material.specular", UniformManager::Vector3);
	uniforms.AddUniform("material.shininess", UniformManager::Float);
	uniforms.AddUniform("material.emissive", UniformManager::Vector4);

	uniforms.AddUniform("light_.ambient", UniformManager::Vector3);
	uniforms.AddUniform("light_.diffuse", UniformManager::Vector3);
	uniforms.AddUniform("light_.lightPos", UniformManager::Vector4);
	uniforms.AddUniform("light_.specular", UniformManager::Vector3);

	uniforms.AddUniform("dirLight.ambient", UniformManager::Vector3);
	uniforms.AddUniform("dirLight.diffuse", UniformManager::Vector3);
	uniforms.AddUniform("dirLight.position", UniformManager::Vector3);
	uniforms.AddUniform("dirLight.specular", UniformManager::Vector3);

	uniforms.AddUniform("color_val", UniformManager::Vector3);
	uniforms.AddUniform("plane", UniformManager::Vector4);
	uniforms.AddUniform("reflect", UniformManager::Int);
	uniforms.AddUniform("refract", UniformManager::Int);

	

	uniforms.AddUniform("numOfDirlights", UniformManager::Int);
	uniforms.AddUniform("numOfPointlights", UniformManager::Int);
	uniforms.AddUniform("numOfSpotlights", UniformManager::Int);

	uniforms.AddUniform("isCalcUVonGPU", UniformManager::Int);
	uniforms.AddUniform("texKind", UniformManager::Int);
	uniforms.AddUniform("fogColor", UniformManager::Vector3);


	uniforms.AddUniform("constant", UniformManager::Float);
	uniforms.AddUniform("linear", UniformManager::Float);
	uniforms.AddUniform("quadratic", UniformManager::Float);

	uniforms.AddUniform("zFar", UniformManager::Float);
	uniforms.AddUniform("zNear", UniformManager::Float);
	uniforms.AddUniform("diffuseColor", UniformManager::Vector3);
	uniforms.AddUniform("ambientColor", UniformManager::Vector3);
	//uniforms("differentCamMatrix", 0);
	uniforms.AddUniform("differentCamMatrix", UniformManager::Int);
	uniforms.AddUniform("inSetting", UniformManager::Int);
	uniforms.AddUniform("plane", UniformManager::Vector4);
	uniforms.AddUniform("reflectOnly", UniformManager::Int);
	uniforms.AddUniform("refractOnly", UniformManager::Int);
	uniforms.AddUniform("fersnelFactor", UniformManager::Float);
}

void Object::IncreScale()
{
	objInfo.scale.x = 0.45f;
	objInfo.scale.y = 0.45f;
}

bool Object::IsElemented()
{
	return mesh->IsElemented();
}

void Object::SetPlane(const Hcoord& planeVec)
{
	planeVector = planeVec;
	uniforms("plane", &planeVector);
}



Mesh* Object::CreateSphere(float radius, int sectorCount, int stackCount)
{
	Mesh* result = new Mesh();
	float PI = 3.141592653589793238f;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	//float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			Vertex vertexVal;
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			//result->PutVertexData(Vector3(x, y, z));
			vertexVal.position = Vector3(x, y, z);
			//result->GetVertexDatasRef().push_back(Vector3(x, y, z));

			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertexVal.normal = Vector3(nx, ny, nz);
			//result->PutNormalData(Vector3(nx, ny, nz));
			//result->GetVertexDatasRef().push_back(vertexVal);
			result->PutVertex(vertexVal);
		}
	}

	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);
		k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				result->PutIndicesData(k1);
				result->PutIndicesData(k2);
				result->PutIndicesData(k1 + 1);
			}

			if (i != (stackCount - 1))
			{
				result->PutIndicesData(k1 + 1);
				result->PutIndicesData(k2);
				result->PutIndicesData(k2 + 1);
			}
		}
	}
	mesh = result;
	return result;
}

void Object::SetShader(Shader* shader)
{
	mesh->SetShader(shader);
	shader_id = shader->GetShaderId();
	Init();
}

void Object::SetNormalDisplayBuffer(Shader* normalPlayShader)
{
	normalPlayVao = new VAO(normalPlayShader);
	normalPlayVao->Init(reinterpret_cast<float*>(mesh->vertexNormalDisplay.data()), static_cast<int>(mesh->vertexNormalDisplay.size()),
		1, std::vector{3});

	faceNormalPlayVao = new VAO(normalPlayShader);
	faceNormalPlayVao->Init(reinterpret_cast<float*>(mesh->faceNormalsDisplay.data()), static_cast<int>(mesh->faceNormalsDisplay.size()),
		1, std::vector{3});
}

void Object::BindNormalDisplayVAO()
{
	if(normalPlayVao != nullptr)
		normalPlayVao->Bind();
}

void Object::BindFaceNormalDisplayVAO()
{
	if (faceNormalPlayVao != nullptr)
		faceNormalPlayVao->Bind();
}

bool Object::IsNormalVAOExist()
{
	return normalPlayVao != nullptr;
}

bool Object::IsFaceNormalVAOExist()
{
	return faceNormalPlayVao != nullptr;
}

void Object::ReInitUniforms(const int& shaderId)
{
	uniforms.Clear();
	uniforms.SetShaderId(shaderId);
	shader_id = shaderId;
	Initialize_Uniform();
}
