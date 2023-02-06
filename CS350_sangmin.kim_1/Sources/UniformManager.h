#pragma once
#include <map>
#include <string>

class UniformManager
{
public:
	enum SendTypes
	{
		None,
		Matrix4x4,
		Vector3,
		Vector4,
		Float,
		Int
	};

	struct Uniform
	{
		SendTypes type;
		unsigned location;
	};
	
	
	UniformManager();
	void SetShaderId(unsigned int id);
	

	/**
	 * \brief Add uniforms to manager.
	 */
	void AddUniform(std::string key, SendTypes type, unsigned int value);
	void AddUniform(std::string key, SendTypes type);

	unsigned int GetUniformLocation(std::string key);
	unsigned int operator[](std::string key);
	void operator()(std::string key, void* val);
	
	void SendUniforms(SendTypes type, std::string name, void* val);
	void Clear();
private:
	std::map<std::string, Uniform> uniforms;
	unsigned int shaderId;
};
