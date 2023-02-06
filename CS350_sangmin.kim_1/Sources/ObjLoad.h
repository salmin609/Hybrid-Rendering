/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjLoad.h
Purpose: Class for loading given obj files.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/


#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "Affine.h"

class ObjLoad
{
public:
	ObjLoad();
	~ObjLoad();
	void ReadObjFile(std::string filePath, Mesh* mesh);
	void ParseObjFile(char* buffer, std::vector<Vertex>& vertexDatas, float& biggestPosVal);
private:
	std::vector<Vector3>    vertexBuffer;
	std::vector<unsigned>     vertexIndices;
	Mesh* pMesh;
};
