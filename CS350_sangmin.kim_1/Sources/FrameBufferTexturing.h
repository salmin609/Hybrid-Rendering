#pragma once
#include <vector>

class Buffer;
class Shader;
class VAO;
class Texture;
class GLMCamera;
class CubeTexture;
class FrameBuffer
{
public:
	FrameBuffer(bool usingRenderBuffer = false);
	unsigned TextureId();
	unsigned DepthTextureId();
	void Bind();
	void UnBind();
	void Use(unsigned vaoId, unsigned shaderId);
	Texture* GetTexture();
	void DrawToFrameBuffer();
	void CheckStatus();
private:
	Texture* texture;
	unsigned depthId;
	unsigned renderId;
	unsigned framebufferId;
	unsigned textureId;

	//VAO* vao;
	unsigned vao;
	Buffer* buf;
	Shader* shader;
	Texture* newTextures[6];
	CubeTexture* skybox;
	std::vector<GLMCamera*> cams;
};