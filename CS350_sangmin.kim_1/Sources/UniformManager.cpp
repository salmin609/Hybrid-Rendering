#include "UniformManager.h"
#include <iostream>
#include <utility>
#include <GL/glew.h>

UniformManager::UniformManager()
{
	
}

void UniformManager::AddUniform(std::string key, SendTypes type, unsigned int value)
{
	uniforms.insert(std::make_pair(key, Uniform{type, value}));
}

void UniformManager::AddUniform(std::string key, SendTypes type)
{
	uniforms.insert(std::make_pair(key, Uniform{ type, static_cast<unsigned int>(glGetUniformLocation(shaderId, key.c_str()))}));
}

unsigned int UniformManager::GetUniformLocation(const std::string key)
{
	const auto mapIt = uniforms.find(key);
	
	if(mapIt != uniforms.end())
	{
		return mapIt->second.location;
	}
	std::cout << "No uniforms found" << std::endl;
	return 0;
}

unsigned UniformManager::operator[](std::string key)
{
	return GetUniformLocation(std::move(key));
}

void UniformManager::operator()(std::string key, void* val)
{
	SendUniforms(uniforms[key].type, key, val);
}


void UniformManager::SendUniforms(SendTypes type, std::string name, void* val)
{
	unsigned location = uniforms[name].location;
	float* valInFloat = static_cast<float*>(val);
	int* valInInt = static_cast<int*>(val);
	switch(type)
	{
	case None: 
		break;
	case Matrix4x4:
		glUniformMatrix4fv(location, 1, GL_TRUE, valInFloat);
		break;
	case Vector3:
		glUniform3fv(location, 1, valInFloat);
		break;
	case Vector4:
		glUniform4fv(location, 1, valInFloat);
		break;
	case Float:
		glUniform1fv(location, 1, valInFloat);
		break;
	case Int:
		glUniform1i(location, *valInInt);
		break;
	default: ;
	}
}

void UniformManager::Clear()
{
	uniforms.clear();
}

void UniformManager::SetShaderId(unsigned id)
{
	shaderId = id;
}
