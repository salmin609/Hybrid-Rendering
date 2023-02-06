#include "Light_Mesh.h"

Light_Mesh::Light_Mesh() : SnubDodecMesh(std::string("LightVertex.glsl"), std::string("LightFragment.glsl"))
{
	Vector3 colorVal{ 1.0f, 1.0f, 1.0f };
	//shader->SendUniformVec3("color_val", &colorVal);
}

Light_Mesh::~Light_Mesh()
{
}
