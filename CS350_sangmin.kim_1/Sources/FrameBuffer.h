#pragma once
#include <string>
#include "Shader_Table.hpp"

class Shader;
class Texture;
class VAO;
class FrameBufferObject
{
public:
	FrameBufferObject(Texture* textureVal = nullptr, std::string vertex = shaderFrameBufferVertex, std::string fragment = shaderToonifyPostProcessFragment);
	void Bind() const;
	void UnBind();
	void Use();
	unsigned GetFrameBufferId();
	unsigned GetTextureColorBufferId();
	void UseFrameBuffer(FrameBufferObject* fboSrc, int srcX, int srcY, int destX, int destY);
	void UseFrameBuffer(unsigned froSrc, int srcX, int srcY, int destX, int destY);
	unsigned ColorAttachMentSlot();
	void GetFboWidthHeight(unsigned& width, unsigned& height);
	void SetViewPort();
	void ResetViewPort();
	~FrameBufferObject();
private:
	unsigned frameBufferId;
	unsigned renderBufferId;
	unsigned colorAttachmentSlot;
	Shader* shader;
	Texture* texture;
	VAO* vao;
	unsigned fboWidth;
	unsigned fboHeight;
};
