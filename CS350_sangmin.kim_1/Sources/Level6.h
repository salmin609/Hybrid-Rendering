#pragma once
#include <vector>

#include "Affine.h"
#include "State.h"
class Object;
class LoadedObj;
class Shader;
class Texture;
class VAO;
class Level6 : public State
{
public:
	Level6();
	~Level6();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;

private:
	struct flock_member
	{
		Vector3 position;
		unsigned int : 32;
		Vector3 velocity;
		unsigned int : 32;
	};
	enum
	{
		WORKGROUP_SIZE = 100,
		NUM_WORKGROUPS = 30,
		FLOCK_SIZE = (NUM_WORKGROUPS * WORKGROUP_SIZE)
	};
	std::vector<flock_member> geometryDatas;
	
	unsigned* flockBuffer;
	unsigned* flockRenderVao;
	unsigned geomertyBuffer;
	Shader* flockingComputeShader;
	Shader* flockingRenderShader;
	unsigned frameIndex = 0;
	LoadedObj* objMesh;
	Object* obj;
};
