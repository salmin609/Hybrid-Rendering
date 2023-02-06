/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Shader_Table.hpp
Purpose: State for displaying Assignment 1 requirments.
Language: c++
Platform: Window x64
Project: sangmin.kim_CS350_1
Author: Sangmin Kim(sangmin.kim)
Creation date: 2022/02/05
End Header --------------------------------------------------------*/

#pragma once
#include <string>

const static std::string shader_simple_vertex = "../Shaders/SimpleVertex.glsl";
const static std::string shader_simple_fragment = "../Shaders/SimpleFragment.glsl";
const static std::string shader_object_vertex = "../Shaders/Vertexshader.glsl";
const static std::string shader_object_fragment = "../Shaders/Fragmentshader.glsl";

const static std::string shaderPhongLightingVertex = "../Shaders/PhongLightingVertex.glsl";
const static std::string shaderPhongLightingFragment = "../Shaders/PhongLightingFragment.glsl";

const static std::string shaderBlinnLightingVertex = "../Shaders/BlinnLightingVertex.glsl";
const static std::string shaderBlinnLightingFragment = "../Shaders/BlinnLightingFragment.glsl";


const static std::string shader_texture_vertex = "../Shaders/Vertexshader_Texture.glsl";
const static std::string shader_texture_fragment = "../Shaders/Fragmentshader_Texture.glsl";
const static std::string shader_light_vertex = "../Shaders/LightVertex.glsl";
const static std::string shader_light_fragment = "../Shaders/LightFragment.glsl";
const static std::string shader_shadow_vertex = "../Shaders/ShadowVertex.glsl";
const static std::string shader_shadow_fragment = "../Shaders/ShadowFragment.glsl";
const static std::string shaderInstanceVertex = "../Shaders/VertexShaderInstance.glsl";
const static std::string shaderInstanceFragment = "../Shaders/FragmentShaderInstance.glsl";
const static std::string shaderSingleColorFragment = "../Shaders/SingleColorFragmentShader.glsl";
const static std::string shaderFrameBufferVertex = "../Shaders/FrameBufferVertex.glsl";
const static std::string shaderFrameBufferFragment = "../Shaders/FrameBufferFragment.glsl";
const static std::string shaderToonifyPostProcessFragment = "../Shaders/ToonifyPostPorcessiongFilterFragment.glsl";
const static std::string shaderSkyboxVertex = "../Shaders/SkyboxVertex.glsl";
const static std::string shaderSkyboxFragment = "../Shaders/SkyboxFragment.glsl";
const static std::string shaderParticleColoredVertex = "../Shaders/ParticleColoredVertex.glsl";
const static std::string shaderParticleColoredFragment = "../Shaders/ParticleColoredFragment.glsl";
const static std::string shaderWaterVertex = "../Shaders/WaterVertex.glsl";
const static std::string shaderWaterFragment = "../Shaders/WaterFragment.glsl";
const static std::string shaderFlockingVertex = "../Shaders/FlockingShaderVertex.glsl";
const static std::string shaderFlockingFragment = "../Shaders/FlockingFragmentShader.glsl";
const static std::string shaderQuadVertex = "../Shaders/QuadVertexShader.vert";
const static std::string shaderQuadFragment = "../Shaders/QuadFragmentShader.frag";
const static std::string shaderNormalDisplayVertex = "../Shaders/NormalDisplayShader.vert";
const static std::string shaderNormalDisplayFragment = "../Shaders/NormalDisplayShader.frag";

const static std::string shaderCompute = "../Shaders/ComputeShader.glsl";
const static std::string shaderComputePrefix = "../Shaders/ComputeShaderPrefix.glsl";
const static std::string shaderComputeBasic = "../Shaders/ComputeShaderBasic.glsl";
const static std::string shaderComputeFlocking = "../Shaders/ComputeShaderFlocking.glsl";
const static std::string shaderComputeRaytracing = "../Shaders/ComputeShaderRaytracing.glsl";

const static std::string shaderHdrBloomFilterVertex = "../Shaders/HdrBloomFilterVertex.glsl";
const static std::string shaderHdrBloomFilterFragment = "../Shaders/HdrBloomFilterFragment.glsl";
const static std::string shaderHdrBloomResolveVertex = "../Shaders/HdrBloomResolveVertex.glsl";
const static std::string shaderHdrBloomResolveFragment = "../Shaders/HdrBloomResolveFragment.glsl";
const static std::string shaderHdrBloomSceneVertex = "../Shaders/HdrBloomSceneVertex.glsl";
const static std::string shaderHdrBloomSceneFragment = "../Shaders/HdrBloomSceneFragment.glsl";

const static std::string shaderGroundTesselationControl = "../Shaders/GroundTesselationControl.glsl";
const static std::string shaderGroundTesselationEvaluation = "../Shaders/GroundTesselationEvaluation.glsl";
const static std::string shaderGroundVertex = "../Shaders/GroundVertex.glsl";
const static std::string shaderGroundFragment = "../Shaders/GroundFragment.glsl";

const static std::string shaderParticleCollisionVertex = "../Shaders/ParticleCollisionVertex.glsl";
const static std::string shaderParticleCollisionFragment = "../Shaders/ParticleCollisionFragment.glsl";
const static std::string shaderParticleCollisionCompute = "../Shaders/ParticleCollisionCompute.glsl";
const static std::string shaderParticleCollisionGeometry = "../Shaders/ParticleCollisionGeometry.glsl";

const static std::string shaderWaterCollisionCompute = "../Shaders/WaterComputeShader.glsl";
const static std::string shaderPointToCubeGeometry = "../Shaders/cubeGeometry.glsl";

const static std::string shaderRaytracingPrepareVertex = "../Shaders/tracePrepareVertex.glsl";
const static std::string shaderRaytracingPrepareFragment = "../Shaders/tracePrepareFragment.glsl";
const static std::string shaderRaytracingVertex = "../Shaders/traceVertex.glsl";
const static std::string shaderRaytracingFragment = "../Shaders/traceFragment.glsl";
const static std::string shaderRaytracingBlitVertex = "../Shaders/traceBlitVertex.glsl";
const static std::string shaderRaytracingBlitFragment = "../Shaders/traceBlitFragment.glsl";

const static std::string shaderParticleCompute = "../Shaders/particleComputeShader.glsl";
const static std::string shaderParticleVertex = "../Shaders/particleVertexShader.glsl";
const static std::string shaderParticleFragment = "../Shaders/particleFragmentShader.glsl";
const static std::string shaderParticleGeometry = "../Shaders/particleGeometry.glsl";

const static std::string shaderFluidVertex = "../Shaders/FluidVertex.glsl";
const static std::string shaderFluidFragment = "../Shaders/FluidFragment.glsl";
const static std::string shaderFluidCompute = "../Shaders/FluidCompute.glsl";
const static std::string shaderFluidCompute2 = "../Shaders/FluidCompute2.glsl";
const static std::string shaderFluidComputeNeighbor = "../Shaders/ComputeShaderFluidNeighbor.glsl";
const static std::string shaderFluidGeometry = "../Shaders/fluidGeometry.glsl";

const static std::string shaderFluidVertex2 = "../Shaders/FluidVertex2.glsl";
const static std::string shaderFluidFragment2 = "../Shaders/FluidFragment2.glsl";

const static std::string shaderOnePointVertex = "../Shaders/VertexOnePoint.glsl";
const static std::string shader2DVertex = "../Shaders/2DVertex.glsl";
const static std::string shader2DFragment = "../Shaders/2DFragment.glsl";

const static std::string shaderGBufferVertex = "../Shaders/GBufferVertex.glsl";
const static std::string shaderGBufferFragment = "../Shaders/GBufferFragment.glsl";

const static std::string shaderDeferredShadingVertex = "../Shaders/DeferredShadingVertex.glsl";
const static std::string shaderDeferredShadingFragment = "../Shaders/DeferredShadingFragment.glsl";

const static std::string normalVertexShader = "../Shaders/vertexShaderNormal.glsl";
const static std::string normalFragmentShader = "../Shaders/fragmentShaderNormal.glsl";
