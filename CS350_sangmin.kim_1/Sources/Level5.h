#pragma once

#include "State.h"
#include "SnubDodecMesh.h"

class Buffer;
class FrameBufferObject;
class Light_Mesh;
class Object;
class Shader;
class Texture;
class VAO;
class LoadedObj;
class Model;
class Level5 : public State
{
public:
	Level5();
	~Level5();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
	void ProgramInit();
private:
	Shader* render;
	Shader* filter;
	Shader* resolve;
	Model* model;
	unsigned renderFbo;
	
	unsigned texDepth;
	Texture* texScene;
	Texture* texBrightPass;

	unsigned filterFbo[2];
	unsigned texFilter[2];

	Buffer* uboTransform;
	Buffer* uboMaterial;
	
	SnubDodecMesh snubMesh;
	VAO* objectVao;
	LoadedObj* sphere;
	int screenWidth;
	int screenHeight;
	enum
	{
		SPHERE_COUNT = 1000,
	};
	struct transforms_t
	{
		Matrix mat_proj;
		Matrix mat_view;
		Matrix mat_model[SPHERE_COUNT];
	};
	struct material
	{
		Vector3 diffuse_color;
		unsigned int : 32;
		Vector3 specular_color;
		float specular_power;
		Vector3 ambient_color;
		unsigned int : 32;
	};
	std::vector<Vector3> randomPos;

	float bloomThreshMin = 1.0f;
	float bloomThreshMax = 1.2f;
};
