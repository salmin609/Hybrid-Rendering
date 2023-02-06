#include "FrameBuffer.h"
#include "Graphic.h"
#include <iostream>

#include "Client.h"
#include "Texture.h"
#include "VAO.h"
FrameBufferObject::FrameBufferObject(Texture* textureVal, std::string vertex , std::string fragment)
{
	colorAttachmentSlot = 0;
	fboWidth = Client::windowWidth;
	fboHeight = Client::windowHeight;

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	shader = new Shader(vertex.c_str(),
		fragment.c_str());

	vao = new VAO(shader);
	vao->Init(quadVertices, 24, 2, std::vector<int>{2, 2});

	int val = 0;
	float val2 = 0.2f;
	float val3 = 5.0f;
	float val4 = 0.f;
	shader->Use();
	shader->SendUniformInt("tex0", &val);
	shader->SendUniformFloat("edge_thres", &val2);
	shader->SendUniformFloat("mouse_x_offset", &val4);
	shader->SendUniformFloat("edge_thres2", &val3);

	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	if (textureVal == nullptr)
	{
		texture = Texture::CreateTextureAttachment(fboWidth, fboHeight);
	}
	else
	{
		texture = textureVal;
	}
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fboWidth, fboHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER, renderBufferId);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error ! frame buffer is not complete" << std::endl;
	}
	else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "frame buffer is complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glEnable(GL_DEPTH_TEST);
}

void FrameBufferObject::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::Use()
{
	//shader->Use();
	////UseFrameBuffer(fboSrc);
	//glBindVertexArray(vao.);
	vao->Bind();
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned FrameBufferObject::GetFrameBufferId()
{
	return frameBufferId;
}

unsigned FrameBufferObject::GetTextureColorBufferId()
{
	return texture->GetTextureId();
}

void FrameBufferObject::UseFrameBuffer(FrameBufferObject* fboSrc, int srcX, int srcY, int destX, int destY)
{
	if (fboSrc != nullptr)
	{
		unsigned srcWidth, srcHeight;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferId);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboSrc->GetFrameBufferId());
		glReadBuffer(fboSrc->GetFrameBufferId());
		fboSrc->GetFboWidthHeight(srcWidth, srcHeight);
		glBlitFramebuffer(0, 0, srcWidth, srcHeight, srcX, srcY, destX, destY,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void FrameBufferObject::UseFrameBuffer(unsigned froSrc, int srcX, int srcY, int destX, int destY)
{

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferId);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, froSrc);
	glReadBuffer(froSrc);
	
	glBlitFramebuffer(0, 0, Client::windowWidth, Client::windowHeight, srcX, srcY, destX, destY,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned FrameBufferObject::ColorAttachMentSlot()
{
	return colorAttachmentSlot;
}

void FrameBufferObject::GetFboWidthHeight(unsigned& width, unsigned& height)
{
	width = fboWidth;
	height = fboHeight;
}

void FrameBufferObject::SetViewPort()
{
	int window_viewport_width = Client::windowWidth;
	int window_viewport_height = Client::windowHeight;

	int x = -(window_viewport_width - static_cast<int>(fboWidth)) / 2;
	int y = -(window_viewport_height - static_cast<int>(fboHeight)) / 2;

	glViewport(x, y, window_viewport_width, window_viewport_height);
}

void FrameBufferObject::ResetViewPort()
{
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &frameBufferId);
}
