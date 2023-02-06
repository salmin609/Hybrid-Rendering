#include "Ground.h"
#include "Graphic.h"
#include "Texture.h"
#include "KtxFileLoader.h"

Texture* Ground::groundHeightMapTexture = nullptr;
Shader* Ground::groundShader = nullptr;
Ground::Ground()
{
	groundShader = new Shader(shaderGroundVertex.c_str(), shaderGroundFragment.c_str(), shaderGroundTesselationControl.c_str(),
		shaderGroundTesselationEvaluation.c_str());
	groundShader->Use();

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	groundShader->SendUniformInt("tex_displacement", 0);
	groundHeightMapTexture = new Texture("height2.png");
	glActiveTexture(GL_TEXTURE1);
	terrianColor = KtxFileLoader::load("terragen_color.ktx");

	Graphic::groundId = groundShader;
}

Ground::~Ground()
{
}
