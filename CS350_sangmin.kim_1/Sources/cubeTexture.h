#ifndef __CUBETEXTURE_HEADER__
#define __CUBETEXTURE_HEADER__

#include <vector>
#include <string>

class Shader;

class CubeTexture
{
public:
	unsigned int m_textureID;

	CubeTexture(std::vector<std::string> faces);
	void Draw(Shader &shader);

private:
	unsigned int m_VAO, m_VBO;

	void initTexture(std::vector<std::string> faces);
	void setupCubeMesh();
};

#endif