#pragma once
#include "Affine.h"

class Shader;


class OutLine
{
public:
	OutLine();
	void OutlinePrepare();
	void Draw();
private:
	Shader* outlineShader;
	
};