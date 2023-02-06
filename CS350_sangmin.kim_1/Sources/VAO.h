/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: VAO.h
Purpose: Helper class for handling VAO in OpenGL
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/


#pragma once
#include <vector>
#include "Affine.h"
class Shader;
class Buffer;

class VAO
{
public:
	
	VAO(Shader* shaderData = nullptr);
	void Init(std::vector<Vertex>& datas, bool onlyPos = false);
	void Init(float* data, int size, int indexNum, const std::vector<int>& sizePerIndex);
	void Bind();
	void SetShader(Shader* shaderData);
	unsigned GetId();
	~VAO();
private:
	unsigned vaoId;
	Shader* shader;
	Buffer* buffer;
};

