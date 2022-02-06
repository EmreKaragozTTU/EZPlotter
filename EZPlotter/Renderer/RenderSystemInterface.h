#pragma once

#include "Math\Math.h"

enum ERenderSystemBackendType
{
	BACKEND_D3D10,
	BACKEND_OGL
};

enum EPrimitiveType
{
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP
};

enum EResourceUsage
{
	STATIC,
	DYNAMIC
};

enum EGraphicsDataFormat
{
	RG_F32_F32,
	RGB_F32_F32_F32, 
	RGBA_F32_F32_F32_F32,
	DS_F24_F8,
	D_F32
};

enum ERasterizerCullMode
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE
};

enum ERasterizerFillMode
{
	WIREFRAME,
	SOLID
};

enum EComparisonFunction
{
	CF_NEVER,
	CF_LESS,
	CF_EQUAL,
	CF_LESS_EQUAL,
	CF_GREATER,
	CF_NOT_EQUAL,
	CF_GREATER_EQUAL,
	CF_ALWAYS
};

enum EBlendFactor
{
	BF_ZERO,
	BF_ONE,
	BF_SRC_COLOR,
	BF_INV_SRC_COLOR,
	BF_SRC_ALPHA,
	BF_INV_SRC_ALPHA,
	BF_DEST_ALPHA,
	BF_INV_DEST_ALPHA,
	BF_DEST_COLOR,
	BF_INV_DEST_COLOR,
	BF_SRC_ALPHA_SAT,
	BF_BLEND_FACTOR,
	BF_INV_BLEND_FACTOR,
	BF_SRC1_COLOR,
	BF_INV_SRC1_COLOR,
	BF_SRC1_ALPHA,
	BF_INV_SRC1_ALPHA
};

enum EBlendMode
{
	BM_ADD,
	BM_SUBTRACT,
	BM_REV_SUBTRACT,
	BM_MIN,
	BM_MAX
};

enum EStencilMode
{
	SM_KEEP,
	SM_ZERO,
	SM_REPLACE,
	SM_INCR_SAT,
	SM_DECR_SAT,
	SM_INVERT,
	SM_INCR,
	SM_DECR
};

struct VertexElement
{
	char* SemanticName;
	EGraphicsDataFormat Format;
	U32 SlotIdx;
	U32 Offset;
};

struct VertexDeclaration
{
	
};

struct VertexBuffer
{
	U32 VertexCount;
	U32 Size;
};

struct IndexBuffer
{
	
};

struct UniformBuffer
{
	
};

struct RenderTarget
{
	U32 Width;
	U32 Height;
};

struct DepthStencilTarget
{
	U32 Width;
	U32 Height;
};

struct VertexShader
{
	

};

struct GeometryShader
{

};

struct PixelShader
{

};

struct RasterizerState
{

};

struct DepthStencilState
{

};

struct BlendState
{

};


class RenderSystemInterface
{
	public:
		// Handle CreateTexture1D();
		// Handle CreateTexture2D();
		// Handle CreateTexture3D();

		virtual VertexDeclaration* CreateVertexDeclaration(const VertexElement* VertexElements, U32 VertexElementCount, const VertexShader* pVertexShader) = 0;

		virtual RenderTarget* CreateRenderTarget(U32 Width, U32 Height, EGraphicsDataFormat Format) = 0;
		virtual DepthStencilTarget* CreateDepthStencilTarget(U32 Width, U32 Height, EGraphicsDataFormat Format) = 0;
		virtual VertexBuffer* CreateVertexBuffer(U32 Size, EResourceUsage Usage, const void* Data) = 0;
		// virtual IndexBuffer* CreateIndexBuffer(U32 NumIndices, U32 Size, U32 Usage, const void* Data) = 0;
		virtual UniformBuffer* CreateUniformBuffer(U32 Size, EResourceUsage Usage, const void* Data) = 0;
		virtual VertexShader* CreateVertexShader(const void* Code, size_t Length) = 0;
		// virtual GeometryShader* CreateGeometryShader(const void* Code, size_t Length) = 0;
		virtual PixelShader* CreatePixelShader(const void* Code, size_t Length) = 0;
		virtual RasterizerState* CreateRasterizerState
		(
			ERasterizerCullMode CullMode, 
			ERasterizerFillMode FillMode, 
			bool bEnableScissorTest, 
			bool bEnableLineAA, 
			bool bEnableMSAA, 
			S32 DepthBias = 0, 
			F32 DepthBiasSlopeScale = 0.0f
		) = 0;
		virtual DepthStencilState* CreateDepthStencilState
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
		) = 0;
		virtual BlendState* CreateBlendState
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
		) = 0;
		

		virtual void SetVertexDeclaration(const VertexDeclaration* pVertexDeclaration) = 0;

		virtual void SetVertexBuffer(const VertexBuffer* pVertexBuffer, U32 SlotIdx, U32 Stride, U32 Offset) = 0;

		virtual void SetVertexShader(const VertexShader* pVertexShader) = 0;
		virtual void SetShaderUniformBuffer(const VertexShader* pVertexShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer) = 0;
		// virtual void SetShaderResources(const VertexShader* pVertexShader) = 0;
		// virtual void SetShaderSamplers(const VertexShader* pVertexShader) = 0;

		// virtual void SetGeometryShader() = 0;
		// virtual void SetGeometryShaderResources() = 0;
		// virtual void SetGeometryShaderSamplers() = 0;

		virtual void SetPixelShader(const PixelShader* pPixelShader) = 0;
		virtual void SetShaderUniformBuffer(const PixelShader* pPixelShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer) = 0;
		// virtual void SetPixelShaderResources(const PixelShader* pPixelShader) = 0;
		// virtual void SetPixelShaderSamplers(const PixelShader* pPixelShader) = 0;
		 
		virtual void SetRenderTarget(const RenderTarget* pRenderTarget, const DepthStencilTarget* pDepthStencilTarget) = 0;
		virtual void SetRasterizerState(const RasterizerState* pRasterizerState) = 0;
		virtual void SetBlendState(const BlendState* pBlendState, U32 SampleMask) = 0;

		virtual void SetViewport(U32 StartX, U32 StartY, U32 Width, U32 Height, F32 ZNear, F32 ZFar) = 0;
		virtual void SetDepthStencilState(const DepthStencilState* pDepthStencilState, U32 StencilRef = 0) = 0;

		virtual void Draw(EPrimitiveType PrimitiveType, U32 VertexCount, U32 StartVertexLocation) = 0;
		// virtual void DrawIndexed() = 0;
		// virtual void DrawInstanced() = 0;
		// virtual void DrawIndexedInstanced() = 0;

		virtual void ClearDepthStencilTarget(const DepthStencilTarget* pDepthStencilTarget, F32 DepthClearColor, U8 StencilClearColor) = 0;
		virtual void ClearRenderTarget(const RenderTarget* pRenderTarget, const Math::ColorRGBA& Color) = 0;

		virtual ERenderSystemBackendType GetBackendType(void) = 0;

};

// A Global pointer for rendering system interface, must be initialized on runtime.
static RenderSystemInterface* GRenderSystemInterfacePtr = nullptr;
