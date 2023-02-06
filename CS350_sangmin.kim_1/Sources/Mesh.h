/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/


#ifndef CS250_MESH_H
#define CS250_MESH_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include "Affine.h"
#include "ParticleInstance.h"
#include "Shader_Table.hpp"
#include "Shader.h"

class Texture;
class VAO;

struct Mesh {

	struct Face {
		unsigned int index1, index2, index3;
		Face(int i = -1, int j = -1, int k = -1)
			: index1(i), index2(j), index3(k) {}
	};

	struct Edge {
		int index1, index2;
		Edge(int i = -1, int j = -1)
			: index1(i), index2(j) {}
	};

	enum class UVType {
		PLANAR_UV = 0,
		CYLINDRICAL_UV,
		SPHERICAL_UV,
		CUBE_MAPPED_UV
	};
	virtual ~Mesh(void);
	virtual int FaceCount(void);
	virtual unsigned Get_VAO_Id();
	virtual unsigned Get_Shader_Id();

	void Initialize(const char* vertexPath, const char* fragmentPath);
	void InitializeTexturedObj(std::string spritePath, std::string vertex = shader_texture_vertex, std::string fragment = shader_texture_fragment);
	void Initialize_Texture(std::string sprite_path);
	void InitializeInstanceObj(std::string spritePath, std::string vertexPath = shaderInstanceVertex, std::string fragPath = shaderInstanceFragment);
	void InitializeColoredParticle(std::string vertexPath, std::string fragmentPath);
	void SetTexture(int slotOffset = 0);
	bool IsElemented();
	unsigned GetElementId();
	Shader* GetShader();
	void Init_VAO();
	void Init_VBO(void* data, unsigned* slot, size_t arr_size, int stride, void* offset, int index, int vec_size, GLenum drawingType = GL_STATIC_DRAW, bool isNormalize = false);
	void Unbind();
	bool IsInstancing();
	int InstancingNum() const;
	std::vector<Vertex> GetVertexDatas();
	std::vector<Vertex>& GetVertexDatasRef();
	void Bind();
	void PutIndicesData(int index);
	void PutVertex(Vertex data);
	void CalculateNormals();
	void CalculateDisplayingNormals();
	void SetShader(Shader* val);
	void PutTexture(Texture* texVal);
	int calcUVs(Mesh::UVType uvType);
	Vector3 getModelCentroid();
	Vector getCentroidVector(Vector3 vVertex);
	Vector2 calcCubeMap(Vector vEntity);
	std::vector<Vector3> vertexNormalDisplay;
	std::vector<Vector3> faceNormalsDisplay;


protected:
	Shader* shader = nullptr;
	unsigned vao_id = 0;
	unsigned vbo_id = 0;
	unsigned color_id = 0;
	unsigned rotateId = 0;
	unsigned elementId = 0;
	unsigned instancingId = 0;
	unsigned matrixId = 0;
	
	std::vector<Texture*> textures;
	std::vector<unsigned int> elements;
	
	std::vector<Vertex> vertexDatas;
	//std::vector<Vector2> vertexUVs;
	std::vector<Vector3> offsetVec;
	
	std::vector<ParticleInstance> particles;
	
	int instancingNum;
	int particleNum;
	bool isQuad = false;
	bool isInstancing = false;
	float timer;
	VAO* vao = nullptr;

	Vector3 boundingBox[2]{ 
		Vector3{ -1.f, -1.f, -1.f },
		Vector3{ 1.f, 1.f, 1.f }};
};


#endif

