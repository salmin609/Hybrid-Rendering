/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <utility>
#include "Mesh.h"
#include "Graphic.h"
#include "SimpleMeshes.h"
#include "VAO.h"
#include "RandomNumGenerator.h"
#include "Texture.h"
unsigned Mesh::Get_VAO_Id()
{
	if(vao != nullptr)
	{
		return vao->GetId();
	}
	return vao_id;
}

unsigned Mesh::Get_Shader_Id()
{
	if(shader != nullptr)
		return shader->GetShaderId();
	return 0;
}


void Mesh::Initialize(const char* vertexPath, const char* fragmentPath)
{
	(vertexPath);
	(fragmentPath);
	//shader = new Shader(vertexPath, fragmentPath);
	vao = new VAO(shader);
	vao->Init(vertexDatas);

	if (!elements.empty())
	{
		glGenBuffers(1, &elementId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
	}
}

void Mesh::InitializeTexturedObj(std::string spritePath, std::string vertex, std::string fragment)
{
	Initialize_Texture(spritePath);
	Initialize(vertex.c_str(), fragment.c_str());
}


void Mesh::InitializeInstanceObj(std::string spritePath, std::string vertexPath, std::string fragPath)
{
	Initialize_Texture(std::move(spritePath));
	Initialize(vertexPath.c_str(), fragPath.c_str());
	instancingNum = 500;
	//const int offsetDivisor = 10;
	//bool minusTrigger = false;

	for (size_t i = 0; i < instancingNum; ++i)
	{
		Vector3 randomVec = RandomNumber::instance->RandomVector3(-10.f, 10.f);
		offsetVec.push_back(randomVec);
	}

	Init_VBO(offsetVec.data(), &instancingId, sizeof(Vector3) * offsetVec.size(),
		0, 0, 3, 3);
	glVertexAttribDivisor(3, 1);
}

void Mesh::InitializeColoredParticle(std::string vertexPath, std::string fragmentPath)
{
	(vertexPath);
	(fragmentPath);
	int simpleCubeSize;
	float* simpleCubeVertices = SimpleMesh::SimpleCube(simpleCubeSize);

	//shader = new Shader(vertexPath.c_str(), fragmentPath.c_str());

	Init_VAO();

	Init_VBO(simpleCubeVertices, &vbo_id, simpleCubeSize * sizeof(float),
		0, 0, 0, 3);

	Init_VBO(NULL, &color_id, particles.size() * sizeof(Vector3),
		0, 0, 1, 3, GL_STREAM_DRAW);

	Init_VBO(NULL, &matrixId, particles.size() * sizeof(Vector3),
		0, 0, 2, 3, GL_STREAM_DRAW);

	Init_VBO(NULL, &rotateId, particles.size() * sizeof(Vector3),
		0, 0, 3, 3, GL_STREAM_DRAW);

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);

	delete[] simpleCubeVertices;
}


void Mesh::Initialize_Texture(std::string sprite_path)
{
	textures.push_back(new Texture(sprite_path));
}


/**
 * \brief
 * Check if texture is exist on the mesh.
 * If so, activate the texture depends on the texture slot's size.
 */
void Mesh::SetTexture(int slotOffset)
{
	const size_t textureSlotSize = textures.size();
	if (textureSlotSize > 0)
	{
		for (unsigned int i = 0; i < textureSlotSize; ++i)
		{
			textures[i]->Bind(i + slotOffset);
		}
	}
}

bool Mesh::IsElemented()
{
	return !elements.empty();
}

unsigned Mesh::GetElementId()
{
	return elementId;
}

Shader* Mesh::GetShader()
{
	return shader;
}

void Mesh::Init_VAO()
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
}

void Mesh::Init_VBO(void* data, unsigned* slot, size_t arr_size, int stride, void* offset, int index, int vec_size, GLenum drawingType
	, bool isNormalize)
{
	if (arr_size > 0)
	{
		glGenBuffers(1, slot);
		glBindBuffer(GL_ARRAY_BUFFER, *slot);
		glBufferData(GL_ARRAY_BUFFER, arr_size, data, drawingType);
		if (!isNormalize)
		{
			glVertexAttribPointer(index, vec_size, GL_FLOAT, GL_FALSE, stride, offset);
		}
		else
		{
			glVertexAttribPointer(index, vec_size, GL_FLOAT, GL_TRUE, stride, offset);
		}
		glEnableVertexAttribArray(index);
	}
	else
	{
		throw std::runtime_error("Error! : Initialize VBO but the size of array is 0");
	}

}

void Mesh::Unbind()
{
	glBindVertexArray(0);

	const size_t texturesSize = textures.size();

	for(size_t i = 0 ; i < texturesSize; ++i)
	{
		textures[i]->Unbind();
	}
}

Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(1, &vbo_id);
	delete shader;

	const size_t texturesSize = textures.size();
	for(size_t i = 0 ; i < texturesSize; ++i)
	{
		delete textures[i];
	}
}

int Mesh::FaceCount()
{
	return static_cast<int>(elements.size());
}


bool Mesh::IsInstancing()
{
	return isInstancing;
}

int Mesh::InstancingNum() const
{
	return instancingNum;
}

std::vector<Vertex> Mesh::GetVertexDatas()
{
	return vertexDatas;
}

std::vector<Vertex>& Mesh::GetVertexDatasRef()
{
	return vertexDatas;
}

void Mesh::Bind()
{
	if(vao != nullptr)
	{
		vao->Bind();
	}
}

void Mesh::PutIndicesData(int index)
{
	elements.push_back(index);
}

void Mesh::PutVertex(Vertex data)
{
	vertexDatas.push_back(data);
}

void Mesh::CalculateNormals()
{
	size_t numFaces = elements.size();
	unsigned numVertices = static_cast<unsigned>(vertexDatas.size());


	//vertexNormals.resize(numVertices, Vector3());
	vertexNormalDisplay.resize(numVertices * 2, Vector3());
	faceNormalsDisplay.resize(numFaces * 2, Vector3());
	//
	//memset(vertexNormals.data(), 0, vertexNormals.size() * sizeof(Vector3));
	memset(vertexNormalDisplay.data(), 0, vertexNormalDisplay.size() * sizeof(Vector3));
	memset(faceNormalsDisplay.data(), 0, faceNormalsDisplay.size() * sizeof(Vector3));

	size_t index = 0;
	unsigned indexFace = 0;
	for (; index < numFaces; )
	{
		unsigned a = elements.at(index++);
		unsigned b = elements.at(index++);
		unsigned c = elements.at(index++);

		Vector3 vA = vertexDatas[a].position;
		Vector3 vB = vertexDatas[b].position;
		Vector3 vC = vertexDatas[c].position;

		Vector vATemp(vA.x, vA.y, vA.z);
		Vector vBTemp(vB.x, vB.y, vB.z);
		Vector vCTemp(vC.x, vC.y, vC.z);

		Vector E1 = vBTemp - vATemp;
		Vector E2 = vCTemp - vATemp;

		Vector N = normalize(cross(E1, E2));

		vertexDatas[a].normal += Vector3(N);
		vertexDatas[b].normal += Vector3(N);
		vertexDatas[c].normal += Vector3(N);

		float midX = (vA.x + vB.x + vC.x) / 3;
		float midY = (vA.y + vB.y + vC.y) / 3;
		float midZ = (vA.z + vB.z + vC.z) / 3;

		faceNormalsDisplay[indexFace] = Vector3(midX, midY, midZ);
		Hcoord val = Vector(midX, midY, midZ) + (N * 0.002f);
		faceNormalsDisplay[indexFace + 1] = Vector3(val.x, val.y, val.z);
		indexFace += 2;
	}

	for (index = 0; index < numVertices; ++index)
	{
		Vector3 vertexNormalsVal = vertexDatas[index].normal;
		Vector3 vertexVal = vertexDatas[index].position;

		Vector normalizedVal = normalize(
			Vector(vertexNormalsVal.x, vertexNormalsVal.y, vertexNormalsVal.z));

		vertexDatas[index].normal = Vector3(normalizedVal.x, normalizedVal.y, normalizedVal.z);
		vertexNormalsVal = vertexDatas[index].normal;

		vertexNormalDisplay[2 * index] = vertexVal;
		vertexNormalDisplay[2 * index + 1] = vertexVal + (vertexNormalsVal * 0.02f);
	}
}

void Mesh::CalculateDisplayingNormals()
{
	size_t numFaces = elements.size();
	unsigned numVertices = static_cast<unsigned>(vertexDatas.size());

	vertexNormalDisplay.resize(numVertices * 2, Vector3());
	faceNormalsDisplay.resize(numFaces * 2, Vector3());
	memset(vertexNormalDisplay.data(), 0, vertexNormalDisplay.size() * sizeof(Vector3));
	memset(faceNormalsDisplay.data(), 0, faceNormalsDisplay.size() * sizeof(Vector3));

	size_t index = 0;
	unsigned indexFace = 0;
	for (; index < numFaces; )
	{
		unsigned a = elements.at(index++);
		unsigned b = elements.at(index++);
		unsigned c = elements.at(index++);

		Vector3 vA = vertexDatas[a].position;
		Vector3 vB = vertexDatas[b].position;
		Vector3 vC = vertexDatas[c].position;

		Vector vATemp(vA.x, vA.y, vA.z);
		Vector vBTemp(vB.x, vB.y, vB.z);
		Vector vCTemp(vC.x, vC.y, vC.z);

		Vector E1 = vBTemp - vATemp;
		Vector E2 = vCTemp - vATemp;

		Vector N = normalize(cross(E1, E2));

		//vertexDatas[a].normal += Vector3(N);
		//vertexDatas[b].normal += Vector3(N);
		//vertexDatas[c].normal += Vector3(N);

		float midX = (vA.x + vB.x + vC.x) / 3;
		float midY = (vA.y + vB.y + vC.y) / 3;
		float midZ = (vA.z + vB.z + vC.z) / 3;

		faceNormalsDisplay[indexFace] = Vector3(midX, midY, midZ);
		Hcoord val = Vector(midX, midY, midZ) + (N * 0.002f);
		faceNormalsDisplay[indexFace + 1] = Vector3(val.x, val.y, val.z);
		indexFace += 2;
	}

	for (index = 0; index < numVertices; ++index)
	{
		Vector3 vertexNormalsVal = vertexDatas[index].normal;
		Vector3 vertexVal = vertexDatas[index].position;

		Vector normalizedVal = normalize(
			Vector(vertexNormalsVal.x, vertexNormalsVal.y, vertexNormalsVal.z));

		//vertexDatas[index].normal = Vector3(normalizedVal.x, normalizedVal.y, normalizedVal.z);
		vertexNormalsVal = Vector3(normalizedVal.x, normalizedVal.y, normalizedVal.z);

		vertexNormalDisplay[2 * index] = vertexVal;
		vertexNormalDisplay[2 * index + 1] = vertexVal + (vertexNormalsVal * 0.002f);
	}
}

Vector3 Mesh::getModelCentroid()
{
	return Vector3(boundingBox[0] + boundingBox[1]) * 0.5f;
}
Vector Mesh::getCentroidVector(Vector3 vVertex)
{
	Vector vVertexVal = Vector(vVertex.x, vVertex.y, vVertex.z);
	Vector3 centroid = getModelCentroid();
	Vector centroidVal = Vector(centroid.x, centroid.y, centroid.z);

	return normalize(vVertexVal - centroidVal);
}

int Mesh::calcUVs(Mesh::UVType uvType)
{
	int rFlag = -1;

	// clear any existing UV
	//vertexUVs.clear();

	//Vector3 delta = getModelScale();
	Vector3 delta = Vector3(1.f, 1.f, 1.f);

	int vectorSize = (int)vertexDatas.size();

	for (int nVertex = 0; nVertex < vectorSize; ++nVertex)
	{
		Vector3 V = vertexDatas[nVertex].position;
		Vector3 N = vertexDatas[nVertex].normal;
		Vector2 uv{ 0.0f, 0.f };

		Vector3 normVertex;

		if(Graphic::instance->isLoadUVwithPos)
		{
			normVertex = Vector3((V.x - boundingBox[0].x) / delta.x,
				(V.y - boundingBox[0].y) / delta.y,
				(V.z - boundingBox[0].z) / delta.z);
		}
		else
		{
			Vector temp = normalize(Vector{ N.x, N.y, N.z });
			normVertex = Vector3{ temp.x, temp.y, temp.z };
		}
		
		//normVertex = abs(normVertex);


		//        normVertex = (2.0f * normVertex) - glm::vec3(1.0f);

		Vector centroidVec = getCentroidVector(V);

		float theta(0.0f);
		float z(0.0f);
		float phi(0.0f);

		switch (uvType)
		{
		case Mesh::UVType::PLANAR_UV:
			uv.x = (normVertex.x - (-1.0f)) / (2.0f);
			uv.y = (normVertex.y - (-1.0f)) / (2.0f);
			break;

		case Mesh::UVType::CYLINDRICAL_UV:
			//theta = glm::degrees(static_cast<float>(atan2(centroidVec.y, centroidVec.x)));
			theta = degrees(static_cast<float>(atan2(centroidVec.y, centroidVec.x)));
			theta += 180.0f;

			z = (centroidVec.z + 1.0f) * 0.5f;

			uv.x = theta / 360.0f;
			uv.y = z;
			break;

		case Mesh::UVType::SPHERICAL_UV:
			theta = degrees(static_cast<float>(atan2(centroidVec.y , centroidVec.x)));
			theta += 180.0f;

			z = centroidVec.z;
			phi = degrees(acos(z / abs(centroidVec)));

			uv.x = theta / 360.0f;
			uv.y = 1.0f - (phi / 180.0f);
			break;

		case Mesh::UVType::CUBE_MAPPED_UV:
			uv = calcCubeMap(centroidVec);
			break;
		}

		//vertexUVs.push_back(uv);
		vertexDatas[nVertex].texCoord = uv;
	}

	return rFlag;
}

Vector2 Mesh::calcCubeMap(Vector vEntity)
{
	float x = vEntity.x;
	float y = vEntity.y;
	float z = vEntity.z;

	float absX = abs(x);
	float absY = abs(y);
	float absZ = abs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	float maxAxis = 0.f;
	float uc = 0.f;
	float vc = 0.f;
	Vector2 uv;

	// POSITIVE X
	if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
	{
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -z;
		vc = y;
	}

	// NEGATIVE X
	else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
	{
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = z;
		vc = y;
	}

	// POSITIVE Y
	else if (bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = x;
		vc = -z;
	}

	// NEGATIVE Y
	else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = x;
		vc = z;
	}

	// POSITIVE Z
	else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = x;
		vc = y;
	}

	// NEGATIVE Z
	else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
	{
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -x;
		vc = y;
	}

	// Convert range from -1 to 1 to 0 to 1
	uv.x = 0.5f * (uc / maxAxis + 1.0f);
	uv.y = 0.5f * (vc / maxAxis + 1.0f);

	return uv;
}

void Mesh::SetShader(Shader* val)
{
	shader = val;
	//vao->SetShader(val);
}

void Mesh::PutTexture(Texture* texVal)
{
	textures.push_back(texVal);
}
