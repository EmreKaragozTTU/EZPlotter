
#include "Plot3DRenderer.h"

const char* PLOT3D_VS_HLSL =
"struct VS_OUTPUT"
"{"
"	float4 Position : SV_POSITION;"
"	float3 WorldPosition : TEXCOORD;"
"};"
""
"cbuffer PER_PLOT : register(b0)"
"{"
"	float4x4 WVPMatrix;"
"};"
""
"VS_OUTPUT Main(float4 Position : POSITION)"
"{"
"	VS_OUTPUT Output;"
"   "
"   Position.w = 1.0f;"
"	Output.Position = mul(Position, WVPMatrix);"
"	Output.WorldPosition = Position.xyz;"
"	return Output;"
"}";

const char* PLOT3D_PS_HLSL =
"struct VS_OUTPUT"
"{"
"	float4 Position : SV_POSITION;"
"	float3 WorldPosition : TEXCOORD;"
"};"
"cbuffer PER_PLOT : register(b0)"
"{"
"	float4 SurfaceColor;"
"};"
""
"float4 Main(VS_OUTPUT Input) : SV_TARGET"
"{"
"	"
"	return float4(  smoothstep( float2(-3, -3), float2(3, 3), Input.WorldPosition.xy), 0.5f, 1.0f );"
"	"
"}";

void Plot3DRenderer::Initialize(RenderSystemInterface* RenderSystemInterfacePtr, const Plot3DRenderData* pRenderData, Plot3DRenderState* pOutRenderState)
{
	pOutRenderState->pVertexShader = RenderSystemInterfacePtr->CreateVertexShader(PLOT3D_VS_HLSL, strlen(PLOT3D_VS_HLSL));
	
	F32 OriginX = (pRenderData->XMax + pRenderData->XMin) / 2.0f;
	F32 OriginDistX = fabsf(pRenderData->XMax - OriginX);

	F32 OriginY = (pRenderData->YMax + pRenderData->YMin) / 2.0f;
	F32 OriginDistY = fabsf(pRenderData->YMax - OriginY);

	F32 OriginZ = (pRenderData->ZMax + pRenderData->ZMin) / 2.0f;
	F32 OriginDistZ = fabsf(pRenderData->ZMax - OriginZ);
	
	Math::Matrix4x4 WorldMatrix = Math::Scale(1.0f / OriginDistX, 1.0f / OriginDistY, 1.0f / (OriginDistZ * 2.0f)) * Math::Translation(0.0f, 0.0f, 0.0f);
	Math::Matrix4x4 ViewMatrix = Math::LookAtMatrixLH(Math::Vector3{ 2.0f, -2.0f, 3.0f }, Math::Vector3{ 0.0f, 0.0f, 0.0f }, Math::Vector3{ 0.0f, 0.0f, 1.0f });
	Math::Matrix4x4 ProjectionMatrix = Math::PerspectiveFovMatrixLH(80.0f, (F32)(pOutRenderState->pRenderTarget->Width) / (F32)(pOutRenderState->pRenderTarget->Height), 0.0f, 1024.0f);

	struct VSUniformBufferData
	{
		Math::Matrix4x4 WVPMatrix;

	} VSUniformBufferData;

	VSUniformBufferData.WVPMatrix = Math::Transpose(WorldMatrix * ViewMatrix * ProjectionMatrix);

	pOutRenderState->pVSUniformBuffer = RenderSystemInterfacePtr->CreateUniformBuffer(sizeof(VSUniformBufferData), EResourceUsage::DYNAMIC, &VSUniformBufferData);


	pOutRenderState->pPixelShader = RenderSystemInterfacePtr->CreatePixelShader(PLOT3D_PS_HLSL, strlen(PLOT3D_PS_HLSL));

	struct PSUniformBufferData
	{
		Math::ColorRGBA SurfaceColor;

	} PSUniformBufferData;

	PSUniformBufferData.SurfaceColor = Math::ColorRGBA{ 0.0f, 0.0f, 1.0f, 1.0f };

	pOutRenderState->pPSUniformBuffer = RenderSystemInterfacePtr->CreateUniformBuffer(sizeof(PSUniformBufferData), EResourceUsage::DYNAMIC, &PSUniformBufferData);

	VertexElement GraphVertexElement;
	GraphVertexElement.SemanticName = "POSITION";
	GraphVertexElement.Format = EGraphicsDataFormat::RGB_F32_F32_F32;
	GraphVertexElement.SlotIdx = 0;
	GraphVertexElement.Offset = 0;

	pOutRenderState->pVertexDeclaration = RenderSystemInterfacePtr->CreateVertexDeclaration(&GraphVertexElement, 1, pOutRenderState->pVertexShader);
	pOutRenderState->pVertexBuffer = RenderSystemInterfacePtr->CreateVertexBuffer(pRenderData->GraphVertexCount * sizeof(Math::Vector3), EResourceUsage::STATIC, &(pRenderData->pGraphVertexArray->operator[](0)));
	pOutRenderState->pVertexBuffer->VertexCount = pRenderData->GraphVertexCount;

	pOutRenderState->pRasterizerState = RenderSystemInterfacePtr->CreateRasterizerState(ERasterizerCullMode::CULL_NONE, ERasterizerFillMode::SOLID, false, true, false);
	pOutRenderState->pBlendState = nullptr;
	pOutRenderState->pDepthStencilState = RenderSystemInterfacePtr->CreateDepthStencilState(true, EComparisonFunction::CF_LESS_EQUAL);
	pOutRenderState->pDepthStencilTarget = RenderSystemInterfacePtr->CreateDepthStencilTarget(pOutRenderState->pRenderTarget->Width, pOutRenderState->pRenderTarget->Height, EGraphicsDataFormat::DS_F24_F8);
}

void Plot3DRenderer::Render(RenderSystemInterface* RenderSystemInterfacePtr, const Plot3DRenderState* pRenderState)
{
	RenderSystemInterfacePtr->ClearRenderTarget(pRenderState->pRenderTarget, Math::ColorRGBA{ 0.0f, 0.0f, 0.0f, 1.0f });
	//RenderSystemInterfacePtr->ClearDepthStencilTarget(pRenderState->pDepthStencilTarget, 1.0f, 0);

	RenderSystemInterfacePtr->SetVertexDeclaration(pRenderState->pVertexDeclaration);
	RenderSystemInterfacePtr->SetVertexBuffer(pRenderState->pVertexBuffer, 0, sizeof(Math::Vector3), 0);
	RenderSystemInterfacePtr->SetVertexShader(pRenderState->pVertexShader);
	RenderSystemInterfacePtr->SetShaderUniformBuffer(pRenderState->pVertexShader, 0, pRenderState->pVSUniformBuffer);
	RenderSystemInterfacePtr->SetPixelShader(pRenderState->pPixelShader);
	RenderSystemInterfacePtr->SetShaderUniformBuffer(pRenderState->pPixelShader, 0, pRenderState->pPSUniformBuffer);
	RenderSystemInterfacePtr->SetRasterizerState(pRenderState->pRasterizerState);
	//RenderSystemInterfacePtr->SetDepthStencilState(pRenderState->pDepthStencilState);
	// RenderSystemInterfacePtr->SetBlendState(pRenderState->pBlendState, 0xFFFFFFFF);
	RenderSystemInterfacePtr->SetRenderTarget(pRenderState->pRenderTarget, /*pRenderState->pDepthStencilTarget*/nullptr);
	RenderSystemInterfacePtr->Draw(EPrimitiveType::POINT_LIST, pRenderState->pVertexBuffer->VertexCount, 0);
}