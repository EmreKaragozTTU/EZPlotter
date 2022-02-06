#pragma once

#include <d3d10.h>
#include <D3Dcompiler.h>

#include <vector>

#include "Common.h"
#include "Math\Math.h"
#include "Renderer\RenderSystemInterface.h"

#define MAX_SWAP_CHAINS 4

typedef U32 Handle;


struct VertexElementD3D10 : public VertexElement
{
	
};


struct VertexDeclarationD3D10 : public VertexDeclaration
{
	ID3D10InputLayout* pInputLayout;
	std::vector<VertexElementD3D10> VertexElements;
};

struct VertexBufferD3D10 : public VertexBuffer
{
	ID3D10Buffer* pBuffer;
};

struct IndexBufferD3D10 : public IndexBuffer
{
	ID3D10Buffer* pBuffer;

	U32 NumIndices;
	U32 Size;
};

struct UniformBufferD3D10 : public UniformBuffer
{
	ID3D10Buffer* pBuffer;

	U32 Size;
};

struct RenderTargetD3D10 : public RenderTarget
{
	ID3D10Texture2D* pTexture;
	ID3D10RenderTargetView* pRenderTargetBinding;
};

struct DepthStencilTargetD3D10 : public DepthStencilTarget
{
	ID3D10Texture2D* pTexture;
	ID3D10DepthStencilView* pDepthStencilTargetBinding;
};


struct VertexShaderD3D10 : public VertexShader
{
	ID3D10VertexShader* pShader;
	ID3D10ShaderResourceView** pShaderResourceBinding;
	
	void* Code;
	U32 CodeLength;

};

struct PixelShaderD3D10 : public PixelShader
{
	ID3D10PixelShader* pShader;
	ID3D10ShaderResourceView** pShaderResourceBindings;
	
};

struct RasterizerStateD3D10 : public RasterizerState
{
	ID3D10RasterizerState* pRasterizerState;
};

struct DepthStencilStateD3D10 : public DepthStencilState
{
	ID3D10DepthStencilState* pDepthStencilState;
};

struct BlendStateD3D10 : public BlendState
{
	ID3D10BlendState* pBlendState;
};



class RenderSystemD3D10 : public RenderSystemInterface
{
	public:
		
		RenderSystemD3D10()
		{

		};


		RenderSystemD3D10(ID3D10Device* pDevice)
		{
			_pDevice_ = pDevice;
		};

		// Handle CreateTexture1D();
		// Handle CreateTexture2D();
		// Handle CreateTexture3D();

		VertexDeclaration* CreateVertexDeclaration(const VertexElement* VertexElements, U32 VertexElementCount, const VertexShader* pVertexShader);

		RenderTarget* CreateRenderTarget(U32 Width, U32 Height, EGraphicsDataFormat Format);

		DepthStencilTarget* CreateDepthStencilTarget(U32 Width, U32 Height, EGraphicsDataFormat Format);

		VertexBuffer* CreateVertexBuffer(U32 Size, EResourceUsage Usage, const void* Data);

		// IndexBuffer* CreateIndexBuffer(U32 NumIndices, U32 Size, U32 Usage, const void* Data);

		UniformBuffer* CreateUniformBuffer(U32 Size, EResourceUsage Usage, const void* Data);
		
		VertexShader* CreateVertexShader(const void* Code, size_t Length);

		//GeometryShader* CreateGeometryShader(const void* Code, size_t Length);

		PixelShader* CreatePixelShader(const void* Code, size_t Length);
		
		RasterizerState* CreateRasterizerState
		(
			ERasterizerCullMode CullMode, 
			ERasterizerFillMode FillMode,
			bool bEnableScissorTest,
			bool bEnableLineAA,
			bool bEnableMSAA,
			S32 DepthBias,
			F32 DepthBiasSlopeScale
		);

		DepthStencilState* CreateDepthStencilState
		(
			bool bEnableDepthTest,
			EComparisonFunction DepthComparisonFunc,
			bool bEnableDepthMask = true,
			bool bEnableStencilTest = false,
			U8 StencilReadMask = 0xff,
			U8 StencilWriteMask = 0xff,
			EStencilMode StencilFrontFailMode = SM_KEEP,
			EStencilMode DepthFrontFailMode = SM_KEEP,
			EStencilMode StencilFrontPassMode = SM_KEEP,
			EComparisonFunction StencilFrontComparisonFunc = CF_ALWAYS,
			EStencilMode StencilBackFailMode = SM_KEEP,
			EStencilMode DepthBackFailMode = SM_KEEP,
			EStencilMode StencilBackPassMode = SM_KEEP,
			EComparisonFunction StencilBackComparisonFunc = CF_ALWAYS
		);

		BlendState* CreateBlendState
		(
			bool bEnableBlend, 
			bool bEnableAlphaToCoverage, 
			EBlendFactor SourceFactorRGB, 
			EBlendFactor DestinationFactorRGB, 
			EBlendMode BlendModeRGB, 
			EBlendFactor SourceFactorAlpha, 
			EBlendFactor DestinationFactorAlpha, 
			EBlendMode BlendModeAlpha, 
			U8 Mask
		);

		void SetVertexDeclaration(const VertexDeclaration* pVertexDeclaration);

		void SetVertexBuffer(const VertexBuffer* pVertexBuffer, U32 SlotIdx, U32 Stride, U32 Offset);

		void SetVertexShader(const VertexShader* pVertexShader);
		void SetShaderUniformBuffer(const VertexShader* pVertexShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer);
		// void SetShaderResources(const VertexShader* pVertexShader);
		// void SetShaderSamplers(const VertexShader* pVertexShader);

		// void SetGeometryShader();
		// void SetGeometryShaderConstants();
		// void SetGeometryShaderResources();
		// void SetGeometryShaderSamplers();

		void SetPixelShader(const PixelShader* pPixelShader);
		void SetShaderUniformBuffer(const PixelShader* pPixelShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer);
		// void SetPixelShaderConstants(const PixelShader* pPixelShader);
		// void SetPixelShaderResources(const PixelShader* pPixelShader);
		// void SetPixelShaderSamplers(const PixelShader* pPixelShader);
		
		void SetRenderTarget(const RenderTarget* pRenderTarget, const DepthStencilTarget* pDepthStencilTarget);
		void SetRasterizerState(const RasterizerState* pRasterizerState);
		void SetBlendState(const BlendState* pBlendState, U32 SampleMask);

		void SetViewport(U32 StartX, U32 StartY, U32 Width, U32 Height, F32 ZNear, F32 ZFar);
		void SetDepthStencilState(const DepthStencilState* pDepthStencilState, U32 StencilRef = 0);

		void Draw(EPrimitiveType PrimitiveType, U32 VertexCount, U32 StartVertexLocation);
		// void DrawIndexed();
		// void DrawInstanced();
		// void DrawIndexedInstanced();

		void ClearDepthStencilTarget(const DepthStencilTarget* pDepthStencilTarget, F32 DepthClearColor, U8 StencilClearColor);
		void ClearRenderTarget(const RenderTarget* pRenderTarget, const Math::ColorRGBA& Color);


		ERenderSystemBackendType GetBackendType(void) { return ERenderSystemBackendType::BACKEND_D3D10; };

	
	private:

		ID3D10Device* _pDevice_;

		static U32 _APIPrimitiveTypes_[5];
		static U32 _APIResourceUsageFlags_[2];
		static U32 _APIRasterizerCullModes_[3];
		static U32 _APIRasterizerFillModes_[2];
		static U32 _APIGraphicsDataFormats_[5];
		static U32 _APIComparisonFunctions_[8];
		static U32 _APIBlendFactors_[17];
		static U32 _APIBlendModes_[5];
		static U32 _APIStencilModes_[8];
};