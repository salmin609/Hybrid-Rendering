
#include <iostream>

#include "cubeTexture.h"
#include "stb/stb_image.h"
#include "Shader.h"

// data
float cube[108] =
{
	// Back face
	-50.0f, -50.0f, -50.0f, // Bottom-left
	50.0f, 50.0f, -50.0f, // top-right
	50.0f, -50.0f, -50.0f, // bottom-right
	50.0f, 50.0f, -50.0f, // top-right
	-50.0f, -50.0f, -50.0f, // bottom-left
	-50.0f, 50.0f, -50.0f, // top-left
	// Front face
	-50.0f, -50.0f, 50.0f, // bottom-left
	50.0f, -50.0f, 50.0f, // bottom-right
	50.0f, 50.0f, 50.0f, // top-right
	50.0f, 50.0f, 50.0f, // top-right
	-50.0f, 50.0f, 50.0f, // top-left
	-50.0f, -50.0f, 50.0f, // bottom-left
	// Left face
	-50.0f, 50.0f, 50.0f, // top-right
	-50.0f, 50.0f, -50.0f, // top-left
	-50.0f, -50.0f, -50.0f, // bottom-left
	-50.0f, -50.0f, -50.0f, // bottom-left
	-50.0f, -50.0f, 50.0f, // bottom-right
	-50.0f, 50.0f, 50.0f, // top-right
	// Right face														   
	50.0f, 50.0f, 50.0f, // top-left							   
	50.0f, -50.0f, -50.0f, // bottom-right	   
	50.0f, 50.0f, -50.0f, // top-right         	   
	50.0f, -50.0f, -50.0f, // bottom-right  
	50.0f, 50.0f, 50.0f, // top-left  
	50.0f, -50.0f, 50.0f, // bottom-left     
	// Bottom face				  
	-50.0f, -50.0f, -50.0f, // top-right
	50.0f, -50.0f, -50.0f, // top-left																															   
	50.0f, -50.0f, 50.0f, // bottom-left											   
	50.0f, -50.0f, 50.0f, // bottom-left																											   
	-50.0f, -50.0f, 50.0f, // bottom-right																									   
	-50.0f, -50.0f, -50.0f, // top-right
	// Top face																														 
	-50.0f, 50.0f, -50.0f, // top-left																																									 
	50.0f, 50.0f, 50.0f, // bottom-right																																									 
	50.0f, 50.0f, -50.0f, // top-right     																																									
	50.0f, 50.0f, 50.0f, // bottom-right																																								 
	-50.0f, 50.0f, -50.0f, // top-left																																								 
	-50.0f, 50.0f, 50.0f, // bottom-left        
};

/* CubeTexture Class Implementation */
CubeTexture::CubeTexture(std::vector<std::string> faces)
{
	initTexture(faces);
	setupCubeMesh();
}

void CubeTexture::Draw(Shader &shader)
{
	shader.Use();

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	glBindVertexArray(0);
}

void CubeTexture::initTexture(std::vector<std::string> faces)
{
	// cube texture
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	// load image and generate texture
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "ERROR::CUBETEXUTE::iniTexture(): Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}

	// set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeTexture::setupCubeMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}