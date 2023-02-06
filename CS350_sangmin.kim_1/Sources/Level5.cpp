#include "Level5.h"
#include "Client.h"
#include "Graphic.h"
#include "Projection.h"
#include "VAO.h"
#include "Object.h"
#include "LoadedObj.h"
#include "Texture.h"
#include "Model.h"
#include "RandomNumGenerator.h"
#include "Buffer.hpp"
Level5::Level5()
{
	screenWidth = Client::windowWidth;
	screenHeight = Client::windowHeight;
	render = new Shader(shaderHdrBloomSceneVertex.c_str(), shaderHdrBloomSceneFragment.c_str());
	filter = new Shader(shaderHdrBloomFilterVertex.c_str(), shaderHdrBloomFilterFragment.c_str());
	resolve = new Shader(shaderHdrBloomResolveVertex.c_str(), shaderHdrBloomResolveFragment.c_str());
	model = new Model(0.f, 2.5f);
	sphere = new LoadedObj("Sphere");

	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	static const GLfloat exposureLUT[20] = { 11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f,  1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f };

	glGenFramebuffers(1, &renderFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFbo);

	texScene = new Texture(GL_RGBA16F);
	texScene->BindTextureToCurrentFrameBuffer(GL_COLOR_ATTACHMENT0);

	texBrightPass = new Texture(GL_RGBA16F);
	texBrightPass->BindTextureToCurrentFrameBuffer(GL_COLOR_ATTACHMENT1);

	glGenTextures(1, &texDepth);
	glBindTexture(GL_TEXTURE_2D, texDepth);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, screenWidth, screenHeight);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texDepth, 0);

	glDrawBuffers(2, buffers);
	
	glGenFramebuffers(2, &filterFbo[0]);
	glGenTextures(2, &texFilter[0]);
	for(int i = 0 ; i < 2; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, filterFbo[i]);
		glBindTexture(GL_TEXTURE_2D, texFilter[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? screenWidth : screenHeight, i ? screenHeight : screenWidth);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texFilter[i], 0);
		glDrawBuffers(1, buffers);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	uboTransform = new Buffer(GL_UNIFORM_BUFFER, (2 + SPHERE_COUNT) * sizeof(Matrix), 
		GL_DYNAMIC_DRAW, NULL);
	
	for (int i = 0; i < SPHERE_COUNT; i++)
	{
		const Vector3 randomVec = RandomNumber::instance->RandomVector3(-5.f, 5.f);
		randomPos.push_back(randomVec);
	}

	uboMaterial = new Buffer(GL_UNIFORM_BUFFER, SPHERE_COUNT * sizeof(material),
		GL_STATIC_DRAW, NULL);

	material* m = static_cast<material*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, SPHERE_COUNT * sizeof(material),
	                                                      GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	float ambient = 0.001f;

	for(int i = 0; i < SPHERE_COUNT; ++i)
	{
		float fi = 3.14159267f * (float)i / 8.0f;
		m[i].diffuse_color = Vector3(sinf(fi) * 0.5f + 0.5f, sinf(fi + 1.345f) * 0.5f + 0.5f, sinf(fi + 2.567f) * 0.5f + 0.5f);
		m[i].specular_color = Vector3(2.8f, 2.8f, 2.9f);
		m[i].specular_power = 30.0f;
		m[i].ambient_color = Vector3(ambient * 0.025f);
		ambient *= 1.012f;
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	std::vector<Vertex> vertices = sphere->GetVertexDatas();
	objectVao = new VAO(render);
	objectVao->Init(vertices);
}

Level5::~Level5()
{
	delete render;
	delete filter;
	delete resolve;
	delete objectVao;
	delete texScene;
	delete texBrightPass;
	delete model;
	delete uboTransform;
	delete uboMaterial;
}

void Level5::Load()
{
	Graphic::objects.clear();
	Graphic::water = nullptr;
	Graphic::groundId = nullptr;
	CameraManager::instance->SetCameraPos(Vector3{ 0.942832f, 1.50537f, -1.57533f }, Vector3{ 0.f, -0.1f, 2.f });
}

void Level5::Update(float dt)
{	
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
	static float total_time = 0.0f;

	total_time += dt;
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, renderFbo);
	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_COLOR, 1, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	objectVao->Bind();
	uboTransform->Bind(1);

	transforms_t* transforms = static_cast<transforms_t*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(transforms_t),
	                                                                       GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
	transforms->mat_proj = CameraToNDC(*CameraManager::instance->GetCamera());
	transforms->mat_proj = transpose(transforms->mat_proj);
	transforms->mat_view = WorldToCamera(*CameraManager::instance->GetCamera());
	transforms->mat_view = transpose(transforms->mat_view);
	
	for(int i = 0; i < SPHERE_COUNT; ++i)
	{
		model->ChangePos(Point(randomPos[i].x, randomPos[i].y, randomPos[i].z));
		Matrix model2World = model->GetModelMatNotTransposed();
		model2World = transpose(model2World);
		transforms->mat_model[i] = model2World;
	}
	glUnmapBuffer(GL_UNIFORM_BUFFER);

	uboMaterial->Bind(2);
	
	render->SendUniformFloat("bloomThreshMin", bloomThreshMin);
	render->SendUniformFloat("bloomThreshMax", bloomThreshMax);
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, sphere->FaceCount() * 3,
		SPHERE_COUNT);

	glDisable(GL_DEPTH_TEST);

	filter->Use();
	//glBindVertexArray(vao);
	
	glBindFramebuffer(GL_FRAMEBUFFER, filterFbo[0]);
	texBrightPass->Bind();
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindFramebuffer(GL_FRAMEBUFFER, filterFbo[1]);
	glBindTexture(GL_TEXTURE_2D, texFilter[0]);
	glViewport(0, 0, Client::windowWidth, Client::windowHeight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	resolve->Use();
	resolve->SendUniformFloat("exposure", 1.f);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texFilter[1]);
	glActiveTexture(GL_TEXTURE0);
	texScene->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Level5::UnLoad()
{
}

void Level5::ProgramInit()
{
	
}
