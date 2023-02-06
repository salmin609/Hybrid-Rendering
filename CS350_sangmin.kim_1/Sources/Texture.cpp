#include "Texture.h"

#include "Client.h"
#include "Image.h"
#include "PPMReader.h"


Texture::Texture(unsigned cubeTextureId, bool isCube)
{
	isCubeMap = isCube;
	textureID = cubeTextureId;

	if (isCube)
		bindingType = GL_TEXTURE_CUBE_MAP;
}

Texture::Texture()
{
	//Create empty texture for cubemapping.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 600,
			600, 0,
			GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	bindingType = GL_TEXTURE_CUBE_MAP;
}

Texture::Texture(GLenum internalFormat)
{
	width = Client::windowWidth;
	height = Client::windowHeight;
	glGenTextures(1, &textureID);
	bindingType = GL_TEXTURE_2D;
	pixelData = nullptr;
	InitParameterI();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
	Unbind();
}
Texture::Texture(const std::string& fileName, bool hasTransparency, GLenum internalFormat) : fileName(fileName)
{
	Image tempImage;

	pixelData = tempImage.Load_Image(fileName, width, height, true);
	this->hasTransparency = hasTransparency;
	glGenTextures(1, &textureID);
	bindingType = GL_TEXTURE_2D;
	InitParameterI();
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

	glGenerateMipmap(GL_TEXTURE_2D);
	Unbind();
}

Texture::Texture(const std::string& ppmFileName, std::vector<Color_Val>& colorVec)
{
	int ppmWidth, ppmHeight, ppmMaxVal;
	readImage(ppmFileName, colorVec, ppmWidth, ppmHeight, ppmMaxVal);
	fileName = ppmFileName;
	width = ppmWidth;
	height = ppmHeight;
	//pixelData = (unsigned char*)colorVec.data();
	bindingType = GL_TEXTURE_2D;
	const size_t vecSize = colorVec.size();
	std::vector<float> convertedColor;
	for (size_t i = 0; i < vecSize; ++i)
	{
		convertedColor.push_back((float)colorVec[i].r / (float)ppmMaxVal);
		convertedColor.push_back((float)colorVec[i].g / (float)ppmMaxVal);
		convertedColor.push_back((float)colorVec[i].b / (float)ppmMaxVal);
	}

	this->hasTransparency = false;
	glGenTextures(1, &textureID);
	InitParameterI();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ppmWidth, ppmHeight, 0, GL_RGB, GL_FLOAT, convertedColor.data());

	glGenerateMipmap(GL_TEXTURE_2D);
	Unbind();
}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

void Texture::GetImageWidthHeight(int& imgWidth, int& imgHeight)
{
	imgWidth = this->width;
	imgHeight = this->height;
}

bool Texture::HasTransparency() {
	return hasTransparency;
}

void Texture::BindImageTexture(int index, GLenum type)
{
	Bind();
	glBindImageTexture(index , textureID, 0, GL_FALSE, 0, type, GL_RGBA32F);
	Unbind();
}

void Texture::BindTextureToCurrentFrameBuffer(GLenum colorAttachmentSlot)
{
	glFramebufferTexture(GL_FRAMEBUFFER, colorAttachmentSlot, textureID, 0);
}

void Texture::InitParameterI()
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Bind(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(bindingType, textureID);
}

void Texture::Unbind() {
	glBindTexture(bindingType, 0);
}

unsigned Texture::GetTextureId()
{
	return textureID;
}

Texture* Texture::CreateTextureAttachment(int width, int height) {
	Texture* texture = new Texture(GL_RGBA);
	texture->width = width, texture->height = height;
	texture->hasTransparency = false;

	glGenTextures(1, &(texture->textureID));
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->textureID, 0);

	return texture;
}
