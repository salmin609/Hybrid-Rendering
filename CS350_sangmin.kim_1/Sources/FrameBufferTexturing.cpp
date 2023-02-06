#include "FrameBufferTexturing.h"

#include <iostream>
#include <SimpleMeshes.h>

#include "Texture.h"
#include <GL/glew.h>

#include "Client.h"
#include "Projection.h"
#include "Skybox.h"
#include "VAO.h"
#include "Buffer.hpp"
#include "glmCamera.h"
#include "cubeTexture.h"
#include <glm/gtc/matrix_transform.hpp>
FrameBuffer::FrameBuffer(bool usingRenderBuffer) : depthId(0)
{
	if (!usingRenderBuffer)
	{
		const int width = Client::windowWidth;
		const int height = Client::windowHeight;
		glGenFramebuffers(1, &framebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);

		texture = new Texture();

		glGenTextures(1, &depthId);
		glBindTexture(GL_TEXTURE_2D, depthId);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->GetTextureId(), 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthId, 0);

		static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
		CheckStatus();
		UnBind();
	}
	else
	{

		glGenFramebuffers(1, &framebufferId);
		texture = new Texture();

		//glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
		Bind();


		//static const GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
		//glDrawBuffers(1, drawBuffers);

		glGenRenderbuffers(1, &depthId);
		glBindRenderbuffer(GL_RENDERBUFFER, depthId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			600, 600);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthId);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
			texture->GetTextureId(), 0);

		static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		//DrawToFrameBuffer();

		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//{
		//	std::cout << "Frame buffer not complete!" << std::endl;
		//}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int size;
	float* simpleCubeVertices = SimpleMesh::SimpleRectangleWithTextureCoord(size);


	//vao = new VAO();
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	buf = new Buffer(GL_ARRAY_BUFFER, size * sizeof(float), GL_STATIC_DRAW, simpleCubeVertices);
	buf->Bind();

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector4), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector4), (void*)sizeof(Vector2));
	glEnableVertexAttribArray(1);


	shader = new Shader(shaderSkyboxVertex.c_str(), 
		shaderSkyboxFragment.c_str());
	newTextures[0] = new Texture("../Images/skybox_right.jpg");
	newTextures[1] = new Texture("../Images/skybox_left.jpg");
	newTextures[2] = new Texture("../Images/skybox_top.jpg");
	newTextures[3] = new Texture("../Images/skybox_bottom.jpg");
	newTextures[4] = new Texture("../Images/skybox_front.jpg");
	newTextures[5] = new Texture("../Images/skybox_back.jpg");

	glm::vec3 insideCameraPos = glm::vec3(0.f, 0.f, -5.f);
	
	GLMCamera* rightCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f);
	GLMCamera* leftCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -180.0f, 0.0f);
	GLMCamera* topCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, 90.0f);
	GLMCamera* bottomCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, -90.0f);
	GLMCamera* backCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), 90.0f, 0.0f);
	GLMCamera* frontCamera = new GLMCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, 0.0f);

	cams = { rightCamera , leftCamera, topCamera, bottomCamera, backCamera, frontCamera };

	std::vector<std::string> textureFaces
	{
		"../Images/skybox_right.jpg",
		"../Images/skybox_left.jpg",
		"../Images/skybox_top.jpg",
		"../Images/skybox_bottom.jpg",
		"../Images/skybox_front.jpg",
		"../Images/skybox_back.jpg",
	};

	skybox = new CubeTexture(textureFaces);


}

void FrameBuffer::DrawToFrameBuffer()
{
	//Camera cam = Camera(Point{ 0.f, 0.f, 0.f }, Vector{ 1.f, 0.f, 0.f }, Vector{ 0.f, 1.f, 0.f }, 0.5f * 3.14159f, 1.f, 0.01f, 1000.f);
	//SkyBox* skyBox = Graphic::instance->GetDrawingManager()->GetSkyBox();
	//Point defaultCamPos = Point(0.f, 0.f, -1.f);
	//Vector ey = Vector{ 0.f, 1.f ,0.f };
	Bind();
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);


	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->GetTextureId(), 0);

		glm::mat4 projection = glm::perspective(glm::radians(cams[i]->Zoom), (float)600 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = cams[i]->GetViewMatrix();
		//glm::mat4 model = glm::mat4();


		//Matrix ndcMat = CameraToNDC(cam);
		//Matrix camMat = WorldToCamera(cam);

		Bind();
		//glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);


		glClearColor(1.f, 1.f, 1.f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glViewport(0, 0, Client::windowWidth, Client::windowHeight);
		glEnable(GL_DEPTH_TEST);

		
		//draw here
		//glDepthFunc(GL_LEQUAL);
		//shader->Use();
		//glBindVertexArray(vao);
		//shader->SendUniformInt("screenTextures", 0);
		//newTextures[i]->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDepthFunc(GL_LESS);


		std::vector<Object*> objs = Graphic::instance->objects;



		for (size_t j = 0; j < objs.size(); ++j)
		{
			if (j == 0)
				continue;
			Object* obj = objs[j];
			Shader* objShader = obj->Get_Mesh()->GetShader();
			objShader->SendUniformInt("differentCamMatrix", 1);
			objShader->SendUniformInt("inSetting", 1);
			//objShader->SendUniformMat("camDifferentAnglendcMat", &ndcMat);
			//objShader->SendUniformMat("camDifferentAnglecamMat", &camMat);
			objShader->SendUniformMatGLM("ndcMat", &projection);
			objShader->SendUniformMatGLM("camMat", &view);

			obj->Draw();
		}

		//glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);
		//shader->Use();
		//glBindVertexArray(vao);
		//shader->SendUniformInt("screenTextures", 0);
		//newTextures[i]->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDepthMask(GL_TRUE);
		//glDepthFunc(GL_LESS);

		//Matrix temp = CameraToNDC(*CameraManager::instance->GetCamera());
		//glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);
		shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->m_textureID);
		skybox->Draw(*shader);
		//glDepthMask(GL_TRUE);
		//glDepthFunc(GL_LESS);
		//glDisable(GL_DEPTH_TEST);
		
	}

	CheckStatus();



	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);

	//shader->Use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->m_textureID);
	//skybox->Draw(*shader);
	//shader->Use();
	//glBindVertexArray(vao);
	//shader->SendUniformInt("screenTextures", 0);
	//newTextures[0]->Bind();
	//glDrawArrays(GL_TRIANGLES, 0, 6);

}

void FrameBuffer::CheckStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_UNDEFINED)
		{
			std::cout << "Frame buffer for environment mapping UNDEFINED" << std::endl;
		}
		else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
		{
			std::cout << "Frame buffer for environment mapping INCOMPLETE ATTACHMENT" << std::endl;
		}
		else if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
		{
			std::cout << "Frame buffer for environment mapping DRAWBUFFER INCOMPLETE" << std::endl;
		}
	}

}

unsigned FrameBuffer::TextureId()
{
	return texture->GetTextureId();
}

unsigned FrameBuffer::DepthTextureId()
{
	return depthId;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferId);
	//texture->Bind();
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CheckStatus();
}

void FrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Use(unsigned vaoId, unsigned shaderId)
{
	glUseProgram(shaderId);
	glBindVertexArray(vaoId);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureId());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Texture* FrameBuffer::GetTexture()
{
	return texture;
}
