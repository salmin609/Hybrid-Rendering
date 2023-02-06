#include "Water.h"

Water::Water()
{
	Water::SetVertices();
}

void Water::SetVertices()
{
	float v[] = { -1.f, -1.f, -1.f,
						1.f, 1.f, -1.f,
						-1.f, 1.f, -1.f,
						-1.f, -1.f, -1.f,
						1.f, -1.f, -1.f,
						1.f, 1.f, 1.f };

	shader = new Shader(shaderWaterVertex.c_str(), shaderWaterFragment.c_str());
	

	Init_VAO();
	Init_VBO(v, &vbo_id, sizeof(v), 0, 0, 0, 3);
	
	shader->SendUniformInt("reflectTexture", 0);
	shader->SendUniformInt("refractTexture", 1);
	shader->SendUniformInt("dudvMap", 2);
	shader->SendUniformInt("normalMap", 3);
	shader->SendUniformInt("depthMap", 4);
}
