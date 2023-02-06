#pragma once
#include "Affine.h"
#include "State.h"
class FrameBufferObject;
class Light_Mesh;
class Object;
class Shader;
class Texture;
class VAO;
class Buffer;
class Level7 : public State
{
public:
	Level7();
	~Level7();
	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
	void Recurse(int depth);
private:
	void InitShader();
	void InitBuffer();
	enum
	{
		MAX_RECURSION_DEPTH = 5,
		MAX_FB_WIDTH = 2048,
		MAX_FB_HEIGHT = 1024
	};
	enum DEBUG_MODE
	{
		DEBUG_NONE,
		DEBUG_REFLECTED,
		DEBUG_REFRACTED,
		DEBUG_REFLECTED_COLOR,
		DEBUG_REFRACTED_COLOR
	};

	struct sphere
	{
		Vector3 center;
		float radius;
		// unsigned int    : 32; // pad
		Vector4 color;
	};

	struct plane
	{
		Vector3 normal;
		float d;
	};

	struct light
	{
		Vector3 position;
		unsigned int : 32;       // pad
	};

	struct uniforms_block
	{
		Matrix mv_matrix;
		Matrix view_matrix;
		Matrix proj_matrix;
	};
	
	Shader* prepareProgram;
	Shader* traceProgram;
	Shader* blitProgram;

	Buffer* uniformBuffer;
	Buffer* sphereBuffer;
	Buffer* planeBuffer;
	Buffer* lightBuffer;

	unsigned rayFbo[MAX_RECURSION_DEPTH];
	unsigned texComposite;
	unsigned texPosition[MAX_RECURSION_DEPTH];
	unsigned texReflected[MAX_RECURSION_DEPTH];
	unsigned texReflectionIntensity[MAX_RECURSION_DEPTH];
	unsigned texRefracted[MAX_RECURSION_DEPTH];
	unsigned texRefractionIntensity[MAX_RECURSION_DEPTH];
	
	VAO* vao;

	int max_depth;
	DEBUG_MODE debugMode;
};
