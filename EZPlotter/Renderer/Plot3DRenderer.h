#pragma once

#include "RenderSystemInterface.h"
#include <vector>

struct Plot3DRenderData
{
	std::vector<Math::Vector3>* pGraphVertexArray;
	U32 GraphVertexCount;
	F32 XMin;
	F32 XMax;
	F32 YMin;
	F32 YMax;
	F32 ZMin;
	F32 ZMax;
};

struct Plot3DRenderState
{
	VertexBuffer* pVertexBuffer;
	UniformBuffer* pVSUniformBuffer;
	UniformBuffer* pPSUniformBuffer;
	VertexDeclaration* pVertexDeclaration;
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
	RasterizerState* pRasterizerState;
	DepthStencilState* pDepthStencilState;
	BlendState* pBlendState;
	RenderTarget* pRenderTarget;
	DepthStencilTarget* pDepthStencilTarget;
};

namespace Plot3DRenderer
{
	void Initialize(RenderSystemInterface* RenderSystemInterfacePtr, const Plot3DRenderData* pRenderData, Plot3DRenderState* pOutRenderState);

	void Render(RenderSystemInterface* RenderSystemInterfacePtr, const Plot3DRenderState* pRenderState);
};