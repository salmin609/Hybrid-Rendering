#pragma once
#include "State.h"

class Model;
class LoadedObj;
class VAO;
class Buffer;
class Shader;
class TgaTexture;

class Level9 : public State
{
public:
	Level9();
	~Level9();

	void Load() override;
	void Update(float dt) override;
	void UnLoad() override;
private:
	struct Particle
	{
		Vector4 currPos;
		Vector4 prevPos;
	};
	
	Shader* render;
	Shader* compute;
	//VAO* vao;
	unsigned vao;
	Buffer* particleBuffer;
	int particleCountX;
	int particleCountY;
	int totalParticleNum;
	int attractorCount;
	Vector3 attrator;
	int gfsCount;
	float fpsTimer;
	bool isActive = true;
	TgaTexture* tgaTexture;
};
