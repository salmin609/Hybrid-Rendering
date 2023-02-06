/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: As1.cpp
Purpose: State for displaying Assignment 1 requirment
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/


#include "As1.h"
#include "Buffer.hpp"
#include "Graphic.h"
#include "ObjLoad.h"
#include "Object.h"
#include "Projection.h"
#include "VAO.h"	
#include "PPMReader.h"
#include "Texture.h"
#include "FrameBufferTexturing.h"
#include "Skybox.h"

As1::As1()
{

	phongShader = new Shader(shader_object_vertex.c_str(), shader_object_fragment.c_str());
	phongShader->Use();
	phongShader->SendUniformInt("skybox", 2);

	phongBlinnShader = new Shader(shaderBlinnLightingVertex.c_str(), shaderBlinnLightingFragment.c_str());
	phongLightingShader = new Shader(shaderPhongLightingVertex.c_str(), shaderPhongLightingFragment.c_str());

	phongLightingShader->SendUniformInt("material.diffuse", 0);
	phongLightingShader->SendUniformInt("material.specular", 1);

	defaultShader = new Shader(shaderQuadVertex.c_str(), shaderQuadFragment.c_str());
	normalDisplayShader = new Shader(shaderNormalDisplayVertex.c_str(), shaderNormalDisplayFragment.c_str());

	Initialize_Material();

	obj = new Object("../Objs/bunny_high_poly.obj", &objMat, phongShader);
	obj->SetPosition(Point(0.f, 0.f, -1.f));

	light = new Object();
	light->SetPosition(Point(0.f, 10.f, 1.f));

	std::vector<Color_Val> colorVec;
	Texture* texture = new Texture("../Images/metal_roof_diff_512x512.ppm", colorVec);

	colorVec.clear();
	Texture* texture2 = new Texture("../Images/metal_roof_spec_512x512.ppm", colorVec);
	obj->Get_Mesh()->PutTexture(texture);
	obj->Get_Mesh()->PutTexture(texture2);


	for (int i = 0; i < 8; ++i)
	{
		spheres.push_back(new Object());
		spheres[i]->InitializeSphereMesh(defaultShader);
		spheres[i]->isLightObj = true;
		spheres[i]->lightKind = LightKinds::DirectionalLight;


		orbits.push_back(new Object());
		orbits[i]->InitializeSphereMesh(defaultShader);
		orbits[i]->isLightObj = true;
		orbits[i]->lightKind = LightKinds::DirectionalLight;
	}



	obj->SetNormalDisplayBuffer(normalDisplayShader);
	orbitLineVao = new VAO(normalDisplayShader);
	const float PI = 3.141592653589793238f;
	const int stackCount = 100;
	const float stackStep = (2 * PI) / stackCount;

	for (float i = 0; i <= stackCount; i += 0.01f)
	{
		float radius = 3.f;
		float stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);             // r * cos(u)
		float z = radius * sinf(stackAngle);              // r * sin(u)
		float x = xy * cosf(1.f);
		//float y = xy * sinf(1.f);

		orbitInfo.emplace_back(x, 0.1f, z - 1.f);
	}
	orbitLineVao->Init(reinterpret_cast<float*>(orbitInfo.data()), static_cast<int>(orbitInfo.size()), 
		1, std::vector{3});

	const int orbitPosOffset = static_cast<int>(orbitInfo.size()) / defaultOrbitsCount;

	for (int i = 0; i < 8; ++i)
	{
		spheres[i]->orbitIndex = i * orbitPosOffset;
	}
	size_t bufferSize = (defaultOrbitsCount * sizeof(DirLight)) + (defaultOrbitsCount * sizeof(PointLight)) + (defaultOrbitsCount * sizeof(SpotLight));

	glGenBuffers(1, &uboLights);
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, bufferSize);

	As1::Load();

	skybox = Graphic::instance->GetDrawingManager()->GetSkyBox();

	Texture* texture3 = new Texture(skybox->textureId, true);
	obj->Get_Mesh()->PutTexture(texture3);

	cam = new Camera(Point{ 0.f, 0.f, 0.f }, Vector{ 1.f, 0.f, 0.f }, Vector{ 0.f, 1.f, 0.f }, 0.5f * PI, 1.f, 0.01f, 1000.f);
}

void As1::ReloadObj(std::string newObjFileName, Mesh::UVType uvType)
{
	Graphic::objects.erase(std::find(Graphic::objects.begin(), Graphic::objects.end(), obj));
	delete obj;
	obj = nullptr;

	std::string newObjFilePath = "../Objs/";
	newObjFilePath += newObjFileName;
	newObjFilePath += ".obj";

	//re-initialize new obj
	obj = new Object(newObjFilePath, &objMat, nullptr, uvType);
	obj->SetPosition(Point(0.f, 0.f, -1.f));
	obj->SetShader(phongShader);
	obj->SetNormalDisplayBuffer(normalDisplayShader);
	Graphic::objects.insert(Graphic::objects.begin(), obj);

}
void As1::ReloadShader(std::string shaderName)
{
	if (shaderName == "PhongLighting")
	{
		obj->SetShader(phongLightingShader);
	}
	else if (shaderName == "PhongShading")
	{
		obj->SetShader(phongShader);
	}
	else if (shaderName == "BlinnShading")
	{
		obj->SetShader(phongBlinnShader);
	}
}

void As1::ToggleLightsInfo()
{
	size_t orbitSize = spheres.size();
	for (int i = 0; i < orbitSize; ++i)
	{
		if (spheres[i]->lightKind == LightKinds::PointLight)
		{
			spheres[i]->lightKind = LightKinds::DirectionalLight;
		}
		else if (spheres[i]->lightKind == LightKinds::DirectionalLight)
		{
			spheres[i]->lightKind = LightKinds::SpotLight;
		}
		else if (spheres[i]->lightKind == LightKinds::SpotLight)
		{
			spheres[i]->lightKind = LightKinds::PointLight;
		}

	}

}

void As1::SetToFirstScene()
{
	ClearAndSetDefaultOrbits();

	const std::vector<Object*>& objs = Graphic::objects;
	const size_t objSize = objs.size();

	Vector3 lightColor = Vector3(0.2f, 0.2f, 0.2f);
	for (size_t i = 0; i < objSize; ++i)
	{
		Object* val = objs[i];
		if (val->isLightObj)
		{
			val->lightKind = LightKinds::PointLight;
			val->lightProperty.ambient = lightColor;
			val->lightProperty.diffuse = lightColor;
			val->lightProperty.specular = lightColor;
		}
	}

	Graphic::instance->globalAtts.x = 0.09f;
	Graphic::instance->globalAtts.y = 1.f;
	Graphic::instance->globalAtts.z = 0.032f;
}

void As1::SetToSecondScene()
{
	ClearAndSetDefaultOrbits();

	const std::vector<Object*>& objs = Graphic::objects;
	const size_t objSize = objs.size();

	Vector3 colorVecs[8];

	colorVecs[0] = Vector3(1.0f, 0.0f, 0.0f);
	colorVecs[1] = Vector3(235.f / 255.f, 115.f / 255.f, 23.f / 255.f);
	colorVecs[2] = Vector3(252.f / 255.f, 245.f / 255.f, 25.f / 255.f);
	colorVecs[3] = Vector3(0.f, 1.f, 0.f);
	colorVecs[4] = Vector3(0.f, 0.f, 1.f);
	colorVecs[5] = Vector3(20.f / 255.f, 71.f / 255.f, 1.f);
	colorVecs[6] = Vector3(139.f / 255.f, 23.f / 255.f, 1.f);
	colorVecs[7] = Vector3(1.f, 1.f, 1.f);


	int colorIndex = 0;
	for (size_t i = 1; i < objSize; ++i)
	{
		Object* val = objs[i];
		if (val->isLightObj)
		{
			val->lightKind = LightKinds::PointLight;

			val->lightProperty.ambient = colorVecs[colorIndex];
			val->lightProperty.diffuse = colorVecs[colorIndex];
			val->lightProperty.specular = colorVecs[colorIndex];

			colorIndex++;
		}
	}
	Graphic::instance->globalAtts.x = 0.09f;
	Graphic::instance->globalAtts.y = 1.f;
	Graphic::instance->globalAtts.z = 0.032f;

}

void As1::SetToThirdScene()
{
	ClearAndSetDefaultOrbits();

	const std::vector<Object*>& objs = Graphic::objects;
	const size_t objSize = objs.size();

	Vector3 colorVecs[8];

	colorVecs[0] = Vector3(1.0f, 0.0f, 0.0f);
	colorVecs[1] = Vector3(235.f / 255.f, 115.f / 255.f, 23.f / 255.f);
	colorVecs[2] = Vector3(252.f / 255.f, 245.f / 255.f, 25.f / 255.f);
	colorVecs[3] = Vector3(0.f, 1.f, 0.f);
	colorVecs[4] = Vector3(0.f, 0.f, 1.f);
	colorVecs[5] = Vector3(20.f / 255.f, 71.f / 255.f, 1.f);
	colorVecs[6] = Vector3(139.f / 255.f, 23.f / 255.f, 1.f);
	colorVecs[7] = Vector3(1.f, 1.f, 1.f);


	int colorIndex = 0;
	for (size_t i = 1; i < objSize; ++i)
	{
		Object* val = objs[i];
		if (val->isLightObj)
		{
			if(i % 2 == 1)
			{
				val->lightKind = LightKinds::DirectionalLight;
			}
			else
			{
				val->lightKind = LightKinds::PointLight;
			}

			val->lightProperty.ambient = colorVecs[colorIndex];
			val->lightProperty.diffuse = colorVecs[colorIndex];
			val->lightProperty.specular = colorVecs[colorIndex];

			colorIndex++;
		}
	}

	objs[3]->lightKind = LightKinds::SpotLight;
	objs[4]->lightKind = LightKinds::SpotLight;
	objs[7]->lightKind = LightKinds::SpotLight;

	Graphic::instance->globalAtts.x = 3.f;
	Graphic::instance->globalAtts.y = 2.67f;
	Graphic::instance->globalAtts.z = -1.4f;
}

void As1::AddLight()
{
	spheres.push_back(new Object());
	size_t lastIndex = spheres.size() - 1;

	spheres[lastIndex]->InitializeSphereMesh(defaultShader);
	spheres[lastIndex]->isLightObj = true;
	spheres[lastIndex]->lightKind = LightKinds::PointLight;

	Graphic::objects.push_back(spheres[lastIndex]);
}

void As1::DeleteLight()
{
	size_t lastIndex = spheres.size() - 1;
	int lightIndex = static_cast<int>(lastIndex);

	for(size_t i = lastIndex; i >= 1; --i)
	{
		if(spheres[i]->isLightObj)
		{
			lightIndex = static_cast<int>(i);
			break;
		}
	}
	spheres.erase(spheres.begin() + lightIndex);

	lastIndex = Graphic::objects.size() - 1;

	for (size_t i = lastIndex; i >= 1; --i)
	{
		if (Graphic::objects[i]->isLightObj)
		{
			lightIndex = static_cast<int>(i);
			break;
		}
	}


	Graphic::objects.erase(Graphic::objects.begin() + lightIndex);
}

void As1::ClearAndSetDefaultOrbits()
{
	Graphic::objects.clear();
	spheres.clear();
	Graphic::objects.push_back(obj);
	const size_t orbitsSize = orbits.size();

	for (size_t i = 0; i < orbitsSize; ++i)
	{
		Graphic::objects.push_back(orbits[i]);
		spheres.push_back(orbits[i]);
	}
	const int orbitPosOffset = static_cast<int>(orbitInfo.size()) / static_cast<int>(orbitsSize);

	for (int i = 0; i < orbitsSize; ++i)
	{
		spheres[i]->orbitIndex = i * orbitPosOffset;
	}

	//Graphic::objects.push_back(plane);
}

void As1::Load()
{
	Graphic::objects.clear();

	if(std::find(Graphic::objects.begin(), Graphic::objects.end(), obj) == Graphic::objects.end())
		Graphic::objects.push_back(obj);

	const size_t sphereSize = spheres.size();

	for (int i = 0; i < sphereSize; ++i)
	{
		if (std::find(Graphic::objects.begin(), Graphic::objects.end(), spheres[i]) == Graphic::objects.end())
			Graphic::objects.push_back(spheres[i]);
		//spheres.push_back(spheres[i]);
		
	}
	//Graphic::objects.push_back(plane);

	CameraManager::instance->SetCameraPos(Vector3{ 0.f, 2.f, 2.f }, Vector3{ 0.f, 0.f, 1.f });
	//Graphic::light = light;
	Graphic::groundId = nullptr;
	Graphic::water = nullptr;

	//Graphic::lights = light;
	SetToFirstScene();
}

void As1::Update(float dt)
{
	(dt);


	normalDisplayShader->Use();

	const std::vector<Object*> objects = Graphic::objects;
	const size_t objSize = objects.size();
	cam = CameraManager::instance->GetCamera();
	Matrix ndcMat = CameraToNDC(*cam);
	Affine camMat = WorldToCamera(*cam);
	Vector3 colorVal{ 0.f, 1.f, 0.f };

	if(isPlayingNormal)
	{
		for(size_t i = 0; i < objSize; ++i)
		{
			if(objects[i]->IsNormalVAOExist())
			{
				objects[i]->BindNormalDisplayVAO();

				Matrix modelMat = objects[i]->Get_Model_To_World();
				Matrix transformMatrix = ndcMat * camMat * modelMat;
				normalDisplayShader->SendUniformMat("vertexTransform", &transformMatrix);

				normalDisplayShader->SendUniform3fv("color", &colorVal, 1);
				const int sizeCheck = static_cast<int>(objects[i]->Get_Mesh()->vertexNormalDisplay.size());

				glDrawArrays(GL_LINES, 0, sizeCheck);
			}
		}
	}

	if(isPlayingFaceNormal)
	{
		colorVal = Vector3{ 0.f, 0.f, 1.f };
		for (size_t i = 0; i < objSize; ++i)
		{
			if (objects[i]->IsFaceNormalVAOExist())
			{
				objects[i]->BindFaceNormalDisplayVAO();

				Matrix modelMat = objects[i]->Get_Model_To_World();
				Matrix transformMatrix = ndcMat * camMat * modelMat;
				normalDisplayShader->SendUniformMat("vertexTransform", &transformMatrix);

				normalDisplayShader->SendUniform3fv("color", &colorVal, 1);
				const int sizeCheck = static_cast<int>(objects[i]->Get_Mesh()->faceNormalsDisplay.size());

				glDrawArrays(GL_LINES, 0, sizeCheck);
			}
		}
	}
	colorVal = Vector3{ 1.f, 1.f, 1.f };
	orbitLineVao->Bind();
	Matrix transformMatrix = ndcMat * camMat;
	normalDisplayShader->SendUniformMat("vertexTransform", &transformMatrix);
	normalDisplayShader->SendUniform3fv("color", &colorVal, 1);
	const int sizeCheck = static_cast<int>(orbitInfo.size());
	glDrawArrays(GL_LINES, 0, sizeCheck);

	const size_t spheresSize = spheres.size();

	for(size_t i = 0; i < spheresSize; ++i)
	{
		Object* objval = spheres[i];
		size_t orbit = objval->orbitIndex;
		objval->orbitIndex += objval->orbitRotatingSpeed;
		if (orbit >= orbitInfo.size())
		{
			objval->orbitIndex = 0;
			orbit = 0;
		}

		const Vector3 orbitPos = orbitInfo[orbit];
		objval->SetPosition(Point(orbitPos.x, orbitPos.y, orbitPos.z));
	}


	const std::vector<Object*>& objs = Graphic::instance->objects;
	const size_t objsSize = objs.size();

	std::vector<PointLight> pointLights;
	std::vector<DirLight> dirLights;
	std::vector<SpotLight> spotLights;

	for (size_t i = 0; i < objsSize; ++i)
	{
		const Object* temp = objs[i];

		if (temp->isLightObj)
		{
			if (temp->lightKind == LightKinds::PointLight)
			{
				Light lightInfo = objs[i]->lightProperty;

				//Vector4 newLightPos = objs[i]->Get_Obj_Pos();
				Point pos = objs[i]->Get_Obj_Pos();
				Vector4 newLightPos = Vector4(pos.x, pos.y, pos.z, 1.f);
				Vector4 newLightAmbient = lightInfo.ambient;
				Vector4 newLightDiffuse = lightInfo.diffuse;
				Vector4 newLightSpec = lightInfo.specular;

				PointLight newPointLight;
				newPointLight.position = newLightPos;
				newPointLight.ambient = newLightAmbient;
				newPointLight.diffuse = newLightDiffuse;
				newPointLight.specular = newLightSpec;


				pointLights.push_back(newPointLight);
			}
			else if (temp->lightKind == LightKinds::DirectionalLight)
			{
				Light lightInfo = objs[i]->lightProperty;

				Point pos = objs[i]->Get_Obj_Pos();
				Vector4 newLightPos = Vector4(pos.x, pos.y, pos.z, 1.f);
				Vector4 newLightAmbient = lightInfo.ambient;
				Vector4 newLightDiffuse = lightInfo.diffuse;
				Vector4 newLightSpec = lightInfo.specular;

				DirLight newDirLight;
				newDirLight.position = newLightPos;
				newDirLight.ambient = newLightAmbient;
				newDirLight.diffuse = newLightDiffuse;
				newDirLight.specular = newLightSpec;

				dirLights.push_back(newDirLight);

			}

			else if (temp->lightKind == LightKinds::SpotLight)
			{
				Light lightInfo = objs[i]->lightProperty;

				Point pos = objs[i]->Get_Obj_Pos();
				Vector4 newLightPos = Vector4(pos.x, pos.y, pos.z, 1.f);
				Vector4 newLightAmbient = lightInfo.ambient;
				Vector4 newLightDiffuse = lightInfo.diffuse;
				Vector4 newLightSpec = lightInfo.specular;

				Vector3 objPos = objs[0]->Get_Obj_Pos();
				Vector4 newLightDir = Vector4(objPos.x, objPos.y, objPos.z, 1.0f) - newLightPos;

				SpotLight newSpotLight;
				newSpotLight.position = newLightPos;
				newSpotLight.direction = newLightDir;
				newSpotLight.ambient = newLightAmbient;
				newSpotLight.diffuse = newLightDiffuse;
				newSpotLight.specular = newLightSpec;
				newSpotLight.cutoffs = 
					Vector4(objs[i]->cutoffs.x, objs[i]->cutoffs.y, 
						0.f, 0.f);

				spotLights.push_back(newSpotLight);

			}
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight) * pointLights.size()
		, pointLights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight) * defaultOrbitsCount, sizeof(DirLight) * dirLights.size()
		, dirLights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight) * (2 * defaultOrbitsCount), sizeof(SpotLight) * spotLights.size()
		, spotLights.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(0);

	//frameBuffers->DrawToFrameBuffer();
	//frameBuffers->Bind();
}

void As1::UnLoad()
{
}

As1::~As1()
{
	if (obj != nullptr)
	{
		delete obj;
	}
	for (int i = 0; i < spheres.size(); ++i)
	{
		delete spheres[i];
	}

//	delete[] spheres;
	delete light;

	delete phongShader;
	delete defaultShader;
	delete normalDisplayShader;
	delete orbitLineVao;
}

void As1::Init_Objects()
{
}

void As1::Initialize_Material()
{
	emerald.ambient = Vector(0.0215f, 0.1745f, 0.0215f);
	emerald.diffuse = Vector(0.07568f, 0.61424f, 0.07568f);
	emerald.specular = Vector(0.633f, 0.727811f, 0.633f);
	emerald.shiness = 20.f;

	obsidian.ambient = Vector(0.05375f, 0.05f, 0.06625f);
	obsidian.diffuse = Vector(0.18275f, 0.17f, 0.22525f);
	obsidian.specular = Vector(0.332741f, 0.328634f, 0.346435f);
	obsidian.shiness = 20.f;

	pearl.ambient = Vector(0.25f, 0.20725f, 0.20725f);
	pearl.diffuse = Vector(1.f, 0.829f, 0.829f);
	pearl.specular = Vector(0.296648f, 0.296648f, 0.296648f);
	pearl.shiness = 20.f;

	objMat.ambient = Vector(0, 0, 13 / 255);
	objMat.diffuse = Vector(.3f,.7f,.6f);
	objMat.specular = Vector(.3f,.2f,.5f);
	objMat.shiness = 10.f;
}

void As1::ChangeDrawingMethod()
{
	Shader* usingShader;

	if (isPlayingPhong)
		usingShader = phongShader;
	else
		usingShader = defaultShader;

	size_t objSize = Graphic::objects.size();
	for (size_t i = 0; i < objSize; ++i)
	{
		Graphic::objects[i]->SetShader(usingShader);
		Graphic::objects[i]->ReInitUniforms(static_cast<int>(usingShader->GetShaderId()));
	}
}
