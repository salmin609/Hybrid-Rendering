#include "Mesh_Object.h"

unsigned Mesh_Object::GetVAOId()
{
	return vao_id;
}

unsigned Mesh_Object::GetShaderId()
{
	return shader->GetShaderId();
}

void Mesh_Object::InitializeVerticesDatas()
{
	const size_t verticesSize = vertices.size();

	for (size_t i = 0; i < verticesSize; ++i)
	{
		Vertex data{ Vector3{ vertices[i].x, vertices[i].y, vertices[i].z } ,
			Vector3{normal[i].x, normal[i].y, normal[i].z}};
		vertexDatas.push_back(data);
	}
}

void Mesh_Object::InitializeTextureObj(std::string sprite_path)
{
	InitializeVerticesDatas();
	InitializeTexturedObj(std::move(sprite_path));
}
