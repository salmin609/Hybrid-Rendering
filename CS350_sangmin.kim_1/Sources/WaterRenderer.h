#pragma once
#include "Affine.h"
class Texture;
class FrameBuffer;

class WaterRenderer
{
public:
	WaterRenderer();
	~WaterRenderer();
	void Render(float dt, Matrix ndcMat, Matrix camMat);
	FrameBuffer* GetReflectTexture();
	FrameBuffer* GetRefractTexture();

private:
	FrameBuffer* waterReflectTexture;
	FrameBuffer* waterRefractTexture;
	Texture* waterDuDv;
	Texture* normalMap;
	float moveSpeed = 0.03f;
	float moveFactor = 0.f;
};