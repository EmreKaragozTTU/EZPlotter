#pragma once

#include "Common.h"
#include "RenderSystemInterface.h"
#include "Backend\D3D10\RenderSystemD3D10.h"

struct Plot2DRenderData
{
	std::vector<Math::Vector3>* pGraphVertexArray;
	U32 GraphVertexCount;
	Math::ColorRGBA LineColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	F32 XMin;
	F32 XMax;
	F32 YMin;
	F32 YMax;
};

struct Plot2DRenderState
{
	// vertex buffers, uniform buffers, vertex declarations etc...

	VertexBuffer* pVertexBuffer;
	UniformBuffer* pVSUniformBuffer;
	UniformBuffer* pPSUniformBuffer;
	VertexDeclaration* pVertexDeclaration;
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	RasterizerState* pRasterizerState;
	RenderTarget* pRenderTarget;
};

namespace Plot2DRenderer
{
	void Initialize(RenderSystemInterface* RenderSystemInterfacePtr, const Plot2DRenderData& RenderData, Plot2DRenderState& OutRenderState);

	void Render(RenderSystemInterface* RenderSystemInterfacePtr, const Plot2DRenderState& RenderState);
	
};