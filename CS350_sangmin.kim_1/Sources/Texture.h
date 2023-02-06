#pragma once
#include <string>
#include <vector>
#include <GL\glew.h>
#include "Affine.h"

struct Color_Val {
	unsigned int r;
	unsigned int g;
	unsigned int b;
};

class Texture {
private:
	std::string fileName;
	unsigned textureID;
	GLsizei width, height;
	bool hasTransparency;
	bool isCubeMap;

public:
	Texture();
	Texture(unsigned cubeTextureId, bool isCube);

	Texture(GLenum internalFormat);
	Texture(const std::string& fileName, bool hasTransparency = false, GLenum internalFormat = GL_RGBA);
	Texture(const std::string& ppmFileName, std::vector<Color_Val>& colorVec);
	~Texture();
	void GetImageWidthHeight(int& imgWidth, int& imgHeight);
	bool HasTransparency();
	void BindImageTexture(int index, GLenum type);
	void BindTextureToCurrentFrameBuffer(GLenum colorAttachmentSlot = GL_COLOR_ATTACHMENT0);
	void InitParameterI();
	void Bind(int index = 0);
	void Unbind();
	unsigned GetTextureId();
	
	static Texture* CreateTextureAttachment(int width, int height);
	
	unsigned char* pixelData;
	GLenum bindingType = GL_TEXTURE_2D;

};
