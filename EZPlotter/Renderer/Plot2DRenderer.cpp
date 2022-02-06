
#include "Plot2DRenderer.h"
#include "Math\Math.h"


const char* PLOT2D_VS_HLSL =
"struct VS_OUTPUT"
"{"
"	float4 Position : SV_POSITION;"
"};"
""
"cbuffer PER_PLOT : register(b0)"
"{"
"	float4x4 TransformMatrix;"
"};"
""
"VS_OUTPUT Main(float4 Position : POSITION)"
"{"
"	VS_OUTPUT Output;"
"   "
"   Position.w = 1.0f;"
"	Output.Position = mul(Position, TransformMatrix);"
""
"	return Output;"
"}";

const char* PLOT2D_PS_HLSL = 
"struct VS_OUTPUT"
"{"
"	float4 Position : SV_POSITION;"
"};"
"cbuffer PER_PLOT : register(b0)"
"{"
"	float4 LineColor;"
"};"
""
"float4 Main() : SV_TARGET"
"{"
"	return LineColor;"
"}";

const char* PLOT2D_VS_GLSL = nullptr;
const char* PLOT2D_PS_GLSL = nullptr;

void Plot2DRenderer::Initialize(RenderSystemInterface* RenderSystemInterfacePtr, const Plot2DRenderData& RenderData, Plot2DRenderState& OutRenderState)
{
	OutRenderState.pVertexShader = RenderSystemInterfacePtr->CreateVertexShader(PLOT2D_VS_HLSL, strlen(PLOT2D_VS_HLSL));
	OutRenderState.pPixelShader = RenderSystemInterfacePtr->CreatePixelShader(PLOT2D_PS_HLSL, strlen(PLOT2D_PS_HLSL));
	
	struct VSUniformBufferData
	{
		Math::Matrix4x4 TransformMatrix;
	} 
	VSUniformBufferData;
	
	F32 OriginX = (RenderData.XMax + RenderData.XMin) / 2.0f;
	F32 OriginDistX = fabsf( RenderData.XMax - OriginX );

	F32 OriginY = (RenderData.YMax + RenderData.YMin) / 2.0f;
	F32 OriginDistY = fabsf(RenderData.YMax - OriginY);

	VSUniformBufferData.TransformMatrix = Math::Scale(1.0f / OriginDistX, 1.0f / OriginDistY, 1.0f) * Math::Translation(-OriginX, -OriginY, 0.0f);
	OutRenderState.pVSUniformBuffer = RenderSystemInterfacePtr->CreateUniformBuffer(sizeof(VSUniformBufferData), EResourceUsage::DYNAMIC, &VSUniformBufferData);
	
	struct PSUniformBufferData
	{
		Math::ColorRGBA LineColor;
	}
	PSUniformBufferData;

	PSUniformBufferData.LineColor = RenderData.LineColor;
	OutRenderState.pPSUniformBuffer = RenderSystemInterfacePtr->CreateUniformBuffer(sizeof(PSUniformBufferData), EResourceUsage::DYNAMIC, &PSUniformBufferData);

	VertexElement GraphVertexElement;
	GraphVertexElement.SemanticName = "POSITION";
	GraphVertexElement.Format = EGraphicsDataFormat::RGB_F32_F32_F32;
	GraphVertexElement.SlotIdx = 0;
	GraphVertexElement.Offset = 0;
	OutRenderState.pVertexDeclaration = RenderSystemInterfacePtr->CreateVertexDeclaration(&GraphVertexElement, 1, OutRenderState.pVertexShader);
	
	// Check this
	OutRenderState.pVertexBuffer = RenderSystemInterfacePtr->CreateVertexBuffer(RenderData.GraphVertexCount * sizeof(Math::Vector3), EResourceUsage::STATIC, &(RenderData.pGraphVertexArray->operator[](0)) );
	OutRenderState.pVertexBuffer->VertexCount = RenderData.GraphVertexCount;

	OutRenderState.pRasterizerState = RenderSystemInterfacePtr->CreateRasterizerState(ERasterizerCullMode::CULL_NONE, ERasterizerFillMode::SOLID, false, true, false);
}

void Plot2DRenderer::Render(RenderSystemInterface* RenderSystemInterfacePtr, const Plot2DRenderState& RenderState)
{
	RenderSystemInterfacePtr->ClearRenderTarget(RenderState.pRenderTarget, Math::ColorRGBA{ 1.0f, 1.0f, 1.0f, 1.0f });

	RenderSystemInterfacePtr->SetVertexBuffer(RenderState.pVertexBuffer, 0, sizeof(Math::Vector3), 0);

	RenderSystemInterfacePtr->SetVertexDeclaration(RenderState.pVertexDeclaration);
	RenderSystemInterfacePtr->SetVertexShader(RenderState.pVertexShader);
	RenderSystemInterfacePtr->SetShaderUniformBuffer(RenderState.pVertexShader, 0, RenderState.pVSUniformBuffer);

	RenderSystemInterfacePtr->SetPixelShader(RenderState.pPixelShader);
	RenderSystemInterfacePtr->SetShaderUniformBuffer(RenderState.pPixelShader, 0, RenderState.pPSUniformBuffer);

	RenderSystemInterfacePtr->SetRasterizerState(RenderState.pRasterizerState);
	//RenderSystemInterfacePtr->SetBlendState(pBlendState, 0xFFFFFFFF);

	// TODO: Move to renderwidget
	// RenderSystemInterfacePtr->SetViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);

	RenderSystemInterfacePtr->SetRenderTarget(RenderState.pRenderTarget, nullptr);

	RenderSystemInterfacePtr->Draw(EPrimitiveType::POINT_LIST, RenderState.pVertexBuffer->VertexCount, 0);
}