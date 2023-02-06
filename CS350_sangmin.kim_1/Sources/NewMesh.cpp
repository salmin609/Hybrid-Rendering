#include "NewMesh.h"

NewMesh::NewMesh(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	shader = new Shader(vertexShaderPath, fragmentShaderPath);


}
