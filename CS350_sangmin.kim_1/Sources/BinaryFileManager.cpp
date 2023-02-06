#include "BinaryFileManager.h"
#include <fstream>
#include <iostream>
#include <iomanip>

void BinaryFileManager::SaveBinFile(std::string fileName, std::vector<Vertex> verticesInfo, bool consistNormal, bool consistTexture)
{
	std::string posSaveLocation = "bin/" + fileName + "position" + ".dat";
	std::string normalSaveLocation = "bin/" + fileName + "normal" + ".dat";
	std::string textureSaveLocation = "bin/" + fileName + "texture" + ".dat";
	
	std::ofstream writeFile(posSaveLocation.data(), std::ios::binary | std::ios::out);
	std::ofstream normalFile;
	std::ofstream textureFile;
	
	if(consistNormal)
	{
		normalFile.open(normalSaveLocation.data(), std::ios::binary | std::ios::out);
	}
	if(consistTexture)
	{
		textureFile.open(textureSaveLocation.data(), std::ios::binary | std::ios::out);
	}
	
	const size_t verticesSize = verticesInfo.size();
	
	if(writeFile.is_open())
	{
		for(size_t i = 0; i < verticesSize; ++i)
		{
			writeFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].position.x)), sizeof(float));
			writeFile << "|";
			writeFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].position.y)), sizeof(float));
			writeFile << "|";
			writeFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].position.z)), sizeof(float));
			writeFile << std::endl;

			if(consistNormal)
			{
				normalFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].normal.x)), sizeof(float));
				normalFile << "|";
				normalFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].normal.y)), sizeof(float));
				normalFile << "|";
				normalFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].normal.z)), sizeof(float));
				normalFile << std::endl;
			}
			if(consistTexture)
			{
				textureFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].texCoord.x)), sizeof(float));
				textureFile << "|";
				textureFile.write(reinterpret_cast<const char*>(&(verticesInfo[i].texCoord.y)), sizeof(float));
				textureFile << std::endl;
			}
		}
		writeFile.close();

		if(consistNormal)
		{
			normalFile.close();
		}
		if(consistTexture)
		{
			textureFile.close();
		}
	}
}

bool BinaryFileManager::LoadDataFile(std::string fileName, std::vector<Vertex>& verticesInfo, int& faceCount)
{
	std::string posPath = "bin/" + fileName + "position.dat";
	std::string normalPath = "bin/" + fileName + "normal.dat";
	std::string texturePath = "bin/" + fileName + "texture.dat";
	
	std::ifstream posIn(posPath.data(), std::ios_base::binary);
	std::ifstream normalIn(normalPath.data(), std::ios_base::binary);
	std::ifstream textureIn(texturePath.data(), std::ios_base::binary);

	float tempVal = 0.f;
	
	if (posIn.is_open())
	{
		while (!posIn.eof())
		{
			Point val;
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.x = tempVal;
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.y = tempVal;
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.z = tempVal;
			posIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			verticesInfo.push_back(Vertex{ Vector3{val.x, val.y, val.z} });
			faceCount++;
		}
		posIn.close();
	}
	else
		return false;

	if(normalIn.is_open())
	{
		int i = 0;
		while (!normalIn.eof())
		{
			Point val;
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.x = tempVal;
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.y = tempVal;
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.z = tempVal;
			normalIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			verticesInfo[i].normal = Vector3{val.x, val.y, val.z};
			i++;
		}
		normalIn.close();
	}

	if(textureIn.is_open())
	{
		int i = 0;
		while (!textureIn.eof())
		{
			Point val;
			textureIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.x = tempVal;
			textureIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			textureIn.read(reinterpret_cast<char*>(&tempVal), sizeof(float));
			val.y = tempVal;
			textureIn.read(reinterpret_cast<char*>(&tempVal), sizeof(char));
			verticesInfo[i].texCoord = Vector2{ val.x, val.y};
			i++;
		}
		textureIn.close();
	}
	return true;
}
