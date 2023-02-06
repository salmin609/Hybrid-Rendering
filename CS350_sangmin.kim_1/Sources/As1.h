/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: As1.h
Purpose: State for displaying Assignment 1 requirment
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#pragma once
#include "Object.h"
#include "State.h"

class Camera;
class VAO;
class FrameBuffer;
class SkyBox;
class As1 : public State
{
public:
	As1();
	virtual void Load();
	virtual void Update(float dt);
	virtual void UnLoad();
	~As1();
	void Init_Objects();
	void Initialize_Material();
	void ChangeDrawingMethod();
	void ReloadObj(std::string newObjFileName, Mesh::UVType uvType = Mesh::UVType::CYLINDRICAL_UV);
	void ReloadShader(std::string shaderName);
	void ToggleLightsInfo();

	void SetToFirstScene();
	void SetToSecondScene();
	void SetToThirdScene();

	void AddLight();
	void DeleteLight();

	void ClearAndSetDefaultOrbits();

	bool isPlayingNormal = false;
	bool isPlayingFaceNormal = false;
	bool isPlayingPhong = true;
	
	//Object* plane;
	Object* obj;

private:
	//Object* spheres;
	//std::pair<Object*, Light>* spheres;

	unsigned char* ppmValue;
	//Object* spheres;
	std::vector<Object*> spheres;


	Object* light;
	FrameBuffer* frameBuffers;
	SkyBox* skybox;
	Camera* cam;

	std::vector<Vector3> orbitInfo;
	std::vector<Object*> orbits;
	Material_ emerald;
	Material_ pearl;
	Material_ obsidian;
	Material_ objMat;

	Shader* phongShader;
	Shader* phongLightingShader;
	Shader* phongBlinnShader;
	Shader* defaultShader;
	Shader* normalDisplayShader;
	VAO* orbitLineVao;
	unsigned int uboLights;

	const int defaultOrbitsCount = 16;
	//int spherePosCountInOrbit = 0;
};
