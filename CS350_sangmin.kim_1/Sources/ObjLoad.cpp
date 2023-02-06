/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjLoad.cpp
Purpose: Class for loading given obj files.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS300_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2021/10/01
End Header --------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS
#include "Mesh.h"
#include <fstream>
#include "ObjLoad.h"
#include <iostream>
#include "Affine.h"

ObjLoad::ObjLoad() : vertexBuffer(0), vertexIndices(0), pMesh(nullptr)
{
}

ObjLoad::~ObjLoad()
{
	vertexBuffer.clear();
	vertexIndices.clear();
	pMesh = nullptr;
}

void ObjLoad::ReadObjFile(std::string filePath, Mesh* mesh)
{
	std::ifstream file;
	file.open(filePath);

	if (file.eof())
	{
		std::cout << "failed to load obj file name : " + filePath << std::endl;
		return;
	}

	pMesh = mesh;
	float biggestVertexPosition = 0.f;

	std::vector<Vertex>& vertexDatas = pMesh->GetVertexDatasRef();

	while(!file.eof())
	{
		char buffer[256] = "\0";
		file.getline(buffer, 256, '\n');

		ParseObjFile(buffer, vertexDatas, biggestVertexPosition);
	}

	const size_t verticesSize = vertexDatas.size();
	for (size_t i = 0; i < verticesSize; ++i)
	{
		vertexDatas[i].position.x /= biggestVertexPosition;
		vertexDatas[i].position.y /= biggestVertexPosition;
		vertexDatas[i].position.z /= biggestVertexPosition;
	}

	mesh->CalculateDisplayingNormals();
}

void ObjLoad::ParseObjFile(char* buffer, std::vector<Vertex>& vertexDatas, float& biggestPosVal)
{
	const char* delims = " \r\n\t";
	char* token = strtok(buffer, delims);
	float temp, x, y, z;
	unsigned firstIndex, secondIndex, thirdIndex;
	//std::vector<Vertex>& vertexDatas = pMesh->GetVertexDatasRef();
	//float biggestVertexPosition = 0.f;

	if (token == nullptr)
		return;

	switch (token[0])
	{
	case 'v':
		if(token[1] == '\0')
		{
			token = strtok(nullptr, delims);
			temp = static_cast<float>(atof(token));
			x = temp;
			token = strtok(nullptr, delims);
			temp = static_cast<float>(atof(token));
			y = temp;
			token = strtok(nullptr, delims);
			temp = static_cast<float>(atof(token));
			z = temp;

			if (x > biggestPosVal)
			{
				biggestPosVal = x;
			}
			if (y > biggestPosVal)
			{
				biggestPosVal = y;
			}
			if (z > biggestPosVal)
			{
				biggestPosVal = z;
			}

			Vertex vertex{ Vector3(x, y, z) };
			vertexDatas.push_back(vertex);
		}
		break;
	case 'f':
		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		firstIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		secondIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		token = strtok(nullptr, delims);
		if (token == nullptr)
			break;
		thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

		pMesh->PutIndicesData(firstIndex);
		pMesh->PutIndicesData(secondIndex);
		pMesh->PutIndicesData(thirdIndex);

		token = strtok(nullptr, delims);

		while (token != nullptr)
		{
			secondIndex = thirdIndex;
			thirdIndex = static_cast<unsigned int&&>(atoi(token) - 1);

			pMesh->PutIndicesData(firstIndex);
			pMesh->PutIndicesData(secondIndex);
			pMesh->PutIndicesData(thirdIndex);

			token = strtok(nullptr, delims);
		}
		break;
	case '#':

		break;
	}



}
