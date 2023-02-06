#include "LoadedObj.h"
#include <iostream>
#include <utility>
#include "ObjLoader.h"
#include "BinaryFileManager.h"

LoadedObj::LoadedObj()
{
}

LoadedObj::LoadedObj(std::string filePath, bool isTextured, bool instancing)
{
	file = std::move(filePath);
	isInstancing = instancing;
	LoadedObj::SetVertices();

	if(isTextured)
	{
		const std::string imageName = file + ".png";

		if(isInstancing)
		{
			InitializeInstanceObj(imageName, shaderInstanceVertex, shaderInstanceFragment);
		}
		else
		{
			InitializeTexturedObj(imageName, shader_texture_vertex, shader_texture_fragment);
		}
	}
	else
	{
		Initialize(shader_object_vertex.c_str(), shader_object_fragment.c_str());
	}
}

void LoadedObj::SetVertices()
{
	const std::string binaryFileSearch = std::string("bin/") + file + "position" + std::string(".dat");

	const std::ifstream in_file(binaryFileSearch);

	if(!in_file)
	{
		const std::string originalFilename = file;
		std::cout << "Binary file does not exist" << std::endl;
		file += ".obj";

		ObjLoader::LoadObjRough(file.c_str(), vertexDatas, faceCount, consistTexture, consistNormal);

		//make binary file
		BinaryFileManager::SaveBinFile(originalFilename, vertexDatas, consistNormal, consistTexture);
	}
	else
	{
		std::cout << "Binary file exist" << std::endl;
		BinaryFileManager::LoadDataFile(file, vertexDatas, faceCount);
	}
}

LoadedObj::~LoadedObj()
{
	
}
