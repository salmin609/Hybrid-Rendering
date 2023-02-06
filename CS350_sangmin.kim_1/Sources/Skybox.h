#pragma once
#include <glm/detail/type_mat.hpp>
#include <vector>

#include "Affine.h"

class Shader;
class Texture;
class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	void Draw(Matrix& ndcMat);
	void Draw(glm::mat4& ndcMat, glm::mat4& camMat);
	unsigned textureId;
private:
	unsigned skyboxVao;
	unsigned skyboxVbo;
	Shader* skyboxShader;

	//Texture* textures;
	std::vector<Texture*> textures;
};