#pragma once

//STD Libs
#include<string>
#include<fstream>
#include<vector>
#include<sstream>

//GLEW
#include"GL/glew.h"

//GLFW

//Own libs
#include "Affine.h"

namespace ObjLoader
{
	void LoadObjRough(const char* fileName, std::vector<Vertex>& vertexDatas, int& faceCount, bool& consistTexture,
		bool& consistNormal)
	{
		std::vector<unsigned int> vertexIndices;
		std::vector<unsigned int> texcoordIndices;
		std::vector<unsigned int> normalIndices;

		std::vector<Point> tempVertices;
		std::vector<Point> tempTexcoords;
		std::vector<Vector> tempNormals;
		std::stringstream ss;
		std::ifstream in_file(fileName);
		std::string line = "";
		std::string prefix = "";
		Vector tempVec3;
		Vector tempVec2;
		GLint temp_glint = 0;
		bool vtExist = false;
		bool vnExist = false;
		int faceConsistingCount = 0;
		float biggestVertexPosition = 0.f;
		//File open error check
		if (!in_file.is_open())
		{
			throw std::runtime_error("Cannot open the filename + " + std::string(fileName));
		}

		//Read one line at a time
		while (std::getline(in_file, line))
		{
			//Get the prefix of the line
			ss.clear();
			ss.str(line);
			ss >> prefix;

			if (prefix == "#")
			{

			}
			else if (prefix == "o")
			{

			}
			else if (prefix == "s")
			{

			}
			else if (prefix == "use_mtl")
			{

			}
			else if (prefix == "v") //Vertex position
			{
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;

				if(tempVec3.x > biggestVertexPosition)
				{
					biggestVertexPosition = tempVec3.x;
				}
				if (tempVec3.y > biggestVertexPosition)
				{
					biggestVertexPosition = tempVec3.y;
				}
				if (tempVec3.z > biggestVertexPosition)
				{
					biggestVertexPosition = tempVec3.z;
				}
				
				tempVertices.emplace_back(tempVec3.x, tempVec3.y, tempVec3.z);
			}
			else if (prefix == "vt")
			{
				ss >> tempVec2.x >> tempVec2.y;
				tempTexcoords.emplace_back(tempVec2.x, tempVec2.y, 0.f);

				if (vtExist == false)
				{
					consistTexture = true;
					vtExist = true;
					faceConsistingCount++;
				}
			}
			else if (prefix == "vn")
			{
				ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
				tempNormals.emplace_back(tempVec3.x, tempVec3.y, tempVec3.z);
				if (vnExist == false)
				{
					consistNormal = true;
					vnExist = true;
					faceConsistingCount++;
				}
			}
			else if (prefix == "f")
			{
				int counter = 0;
				faceCount++;
				std::vector<unsigned int> tempV;
				std::vector<unsigned int> tempT;
				std::vector<unsigned int> tempN;

				while (ss >> temp_glint)
				{
					//Pushing indices into correct arrays
					if (counter == 0)
					{
						tempV.push_back(temp_glint);
					}
					else if (counter == 1)
					{
						tempT.push_back(temp_glint);
					}
					else if (counter == 2)
					{
						tempN.push_back(temp_glint);
					}

					//Handling characters
					if (ss.peek() == '/')
					{
						++counter;
						ss.ignore(1, '/');
					}
					else if (ss.peek() == ' ')
					{
						++counter;
						ss.ignore(1, ' ');
					}

					//Reset the counter
					if (counter > faceConsistingCount)
						counter = 0;
				}

				if (tempV.size() > 3)
				{
					faceCount++;
					vertexIndices.push_back(tempV[0]);
					vertexIndices.push_back(tempV[1]);
					vertexIndices.push_back(tempV[2]);

					vertexIndices.push_back(tempV[0]);
					vertexIndices.push_back(tempV[2]);
					vertexIndices.push_back(tempV[3]);

					if (!tempT.empty())
					{
						texcoordIndices.push_back(tempT[0]);
						texcoordIndices.push_back(tempT[1]);
						texcoordIndices.push_back(tempT[2]);

						texcoordIndices.push_back(tempT[0]);
						texcoordIndices.push_back(tempT[2]);
						texcoordIndices.push_back(tempT[3]);
					}

					if (!tempN.empty())
					{
						normalIndices.push_back(tempN[0]);
						normalIndices.push_back(tempN[1]);
						normalIndices.push_back(tempN[2]);

						normalIndices.push_back(tempN[0]);
						normalIndices.push_back(tempN[2]);
						normalIndices.push_back(tempN[3]);
					}
				}
				else
				{
					if (!tempV.empty())
					{
						vertexIndices.push_back(tempV[0]);
						vertexIndices.push_back(tempV[1]);
						vertexIndices.push_back(tempV[2]);
					}

					if (!tempT.empty())
					{
						texcoordIndices.push_back(tempT[0]);
						texcoordIndices.push_back(tempT[1]);
						texcoordIndices.push_back(tempT[2]);
					}
					if (!tempN.empty())
					{
						normalIndices.push_back(tempN[0]);
						normalIndices.push_back(tempN[1]);
						normalIndices.push_back(tempN[2]);
					}
				}
			}
			else
			{

			}
		}

		//const size_t indexSize = vertexIndices.size();
		const size_t tempVerticesSize = tempVertices.size();
		const size_t tempNormalSize = tempNormals.size();
		const size_t tempTexcoordSize = tempTexcoords.size();
		const size_t vertexIndexSize = vertexIndices.size();
		const size_t normalIndexSize = normalIndices.size();
		const size_t texCoordIndexSize = texcoordIndices.size();

		for(size_t i = 0; i < tempVerticesSize; ++i)
		{
			tempVertices[i] /= biggestVertexPosition;
		}
		
		vertexDatas.resize(vertexIndexSize, Vertex());
		const size_t vertexDatasSize = vertexDatas.size();

		for (size_t i = 0; i < vertexDatasSize; ++i)
		{
			unsigned int vertexIndex = 0;
			unsigned int normalIndex = 0;
			unsigned int texCoordIndex = 0;

			if (vertexIndexSize > 0)
			{
				vertexIndex = vertexIndices[i] - 1;
			}
			if (normalIndexSize > 0)
			{
				normalIndex = normalIndices[i] - 1;
			}
			if (texCoordIndexSize > 0)
			{
				texCoordIndex = texcoordIndices[i] - 1;
			}

			if (tempVerticesSize > 0)
			{
				Point vertex = tempVertices[vertexIndex];
				vertexDatas[i].position += vertex;
			}
			if (tempNormalSize > 0)
			{
				Vector normal = tempNormals[normalIndex];
				vertexDatas[i].normal += normal;
			}
			if (tempTexcoordSize > 0)
			{
				Point texCoord = tempTexcoords[texCoordIndex];
				vertexDatas[i].texCoord += texCoord;
			}
		}
	}
}
