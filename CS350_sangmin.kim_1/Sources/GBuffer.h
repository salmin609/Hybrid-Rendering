/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: GBuffer.h
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};
	GBuffer();
	~GBuffer();
	void Bind();
	
	void BindForWriting();
	void BindForReading();
	void SetReadBuffer(GBUFFER_TEXTURE_TYPE type);

public:
	unsigned int gBuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedoSpec;
	unsigned int gDepth;
	unsigned int gUV;

	unsigned int rboDepth;
};