#include "Skybox.h"
#include "Graphic.h"
#include "Projection.h"
#include "SimpleMeshes.h"
#include "Image.h"
SkyBox::SkyBox()
{
	skyboxShader = new Shader(shaderSkyboxVertex.c_str(),
		shaderSkyboxFragment.c_str());
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height;
	unsigned char* data;
	/*std::vector<std::string> textureFaces
	{
		"../Images/alldim.png",
		"../Images/alldim.png",
		"../Images/alldim.png",
		"../Images/alldim.png",
		"../Images/front2.png",
		"../Images/alldim.png",
	};*/
	std::vector<std::string> textureFaces
	{
		"../Images/skybox_right.jpg",
		"../Images/skybox_left.jpg",
		"../Images/skybox_top.jpg",
		"../Images/skybox_bottom.jpg",
		"../Images/skybox_front.jpg",
		"../Images/skybox_back.jpg",
	};


	const unsigned int textureSize = static_cast<unsigned int>(textureFaces.size());
	Image tempImg;
	for (unsigned int i = 0; i < textureSize; ++i)
	{
		data = tempImg.Load_Image(textureFaces[i], width, height, false);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<GLenum>(i),
			0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	int size;
	float* simpleCubeVertices = SimpleMesh::SimpleCube(size);
	
	glGenVertexArrays(1, &skyboxVao);
	glGenBuffers(1, &skyboxVbo);
	glBindVertexArray(skyboxVao);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, simpleCubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	int val = 0;
	skyboxShader->Use();
	skyboxShader->SendUniformInt("skybox", &val);

	delete[] simpleCubeVertices;
}

SkyBox::~SkyBox()
{
	
}

void SkyBox::Draw(Matrix& ndcMat)
{
	glDepthFunc(GL_LEQUAL);
	skyboxShader->Use();
	Matrix check = WorldToCameraWithoutTranslation(*CameraManager::instance->GetCamera());
	skyboxShader->SendUniformMat("cam", &check);
	skyboxShader->SendUniformMat("projection", &ndcMat);

	glBindVertexArray(skyboxVao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::Draw(glm::mat4& ndcMat, glm::mat4& camMat)
{
	glDepthFunc(GL_LEQUAL);
	skyboxShader->Use();
	//Matrix check = WorldToCameraWithoutTranslation(*CameraManager::instance->GetCamera());
	skyboxShader->SendUniformMatGLM("cam", &camMat);
	skyboxShader->SendUniformMatGLM("projection", &ndcMat);

	glBindVertexArray(skyboxVao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
