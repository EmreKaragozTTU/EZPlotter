#include "RenderSystemD3D10.h"

U32 RenderSystemD3D10::_APIPrimitiveTypes_[5] =
{
	D3D10_PRIMITIVE_TOPOLOGY_POINTLIST,
	D3D10_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
};

U32 RenderSystemD3D10::_APIResourceUsageFlags_[2] =
{
	D3D10_USAGE_DEFAULT,
	D3D10_USAGE_DYNAMIC
};

U32 RenderSystemD3D10::_APIRasterizerCullModes_[3] = 
{
	D3D10_CULL_BACK,
	D3D10_CULL_FRONT,
	D3D10_CULL_NONE
};

U32 RenderSystemD3D10::_APIRasterizerFillModes_[2] =
{
	D3D10_FILL_WIREFRAME,
	D3D10_FILL_SOLID
};

U32 RenderSystemD3D10::_APIGraphicsDataFormats_[5] =
{
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT
};

U32 RenderSystemD3D10::_APIComparisonFunctions_[8] =
{
	D3D10_COMPARISON_NEVER,
	D3D10_COMPARISON_LESS,
	D3D10_COMPARISON_EQUAL,
	D3D10_COMPARISON_LESS_EQUAL,
	D3D10_COMPARISON_GREATER,
	D3D10_COMPARISON_NOT_EQUAL,
	D3D10_COMPARISON_GREATER_EQUAL,
	D3D10_COMPARISON_ALWAYS
};

U32 RenderSystemD3D10::_APIBlendFactors_[17] =
{
	D3D10_BLEND_ZERO,
	D3D10_BLEND_ONE,
	D3D10_BLEND_SRC_COLOR,
	D3D10_BLEND_INV_SRC_COLOR,
	D3D10_BLEND_SRC_ALPHA,
	D3D10_BLEND_INV_SRC_ALPHA,
	D3D10_BLEND_DEST_ALPHA,
	D3D10_BLEND_INV_DEST_ALPHA,
	D3D10_BLEND_DEST_COLOR,
	D3D10_BLEND_INV_DEST_COLOR,
	D3D10_BLEND_SRC_ALPHA_SAT,
	D3D10_BLEND_BLEND_FACTOR,
	D3D10_BLEND_INV_BLEND_FACTOR,
	D3D10_BLEND_SRC1_COLOR,
	D3D10_BLEND_INV_SRC1_COLOR,
	D3D10_BLEND_SRC1_ALPHA,
	D3D10_BLEND_INV_SRC1_ALPHA
};

U32 RenderSystemD3D10::_APIBlendModes_[5] = 
{
	D3D10_BLEND_OP_ADD,
	D3D10_BLEND_OP_SUBTRACT,
	D3D10_BLEND_OP_REV_SUBTRACT,
	D3D10_BLEND_OP_MIN,
	D3D10_BLEND_OP_MAX
};

U32 RenderSystemD3D10::_APIStencilModes_[8] = 
{
	D3D10_STENCIL_OP_KEEP,
	D3D10_STENCIL_OP_ZERO,
	D3D10_STENCIL_OP_REPLACE,
	D3D10_STENCIL_OP_INCR_SAT,
	D3D10_STENCIL_OP_DECR_SAT,
	D3D10_STENCIL_OP_INVERT,
	D3D10_STENCIL_OP_INCR,
	D3D10_STENCIL_OP_DECR
};

// Assuming geometry instancing will never be used. 
// TODO: pVertexShader is always null in OpenGL no need
VertexDeclaration* RenderSystemD3D10::CreateVertexDeclaration(const VertexElement* VertexElements, U32 VertexElementCount, const VertexShader* pVertexShader)
{
	const VertexElementD3D10* APIVertexElements = static_cast<const VertexElementD3D10*>(VertexElements);

	const VertexShaderD3D10* pAPIVertexShader = static_cast<const VertexShaderD3D10*>(pVertexShader);

	std::vector<D3D10_INPUT_ELEMENT_DESC> DescArray;
	DescArray.reserve(VertexElementCount);

	for (U32 VertexElementIdx = 0; VertexElementIdx < VertexElementCount; VertexElementIdx++)
	{
		D3D10_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = APIVertexElements[VertexElementIdx].SemanticName;
		Desc.SemanticIndex = 0;
		Desc.Format = static_cast<DXGI_FORMAT>(_APIGraphicsDataFormats_[APIVertexElements[VertexElementIdx].Format]);
		Desc.InputSlot = APIVertexElements[VertexElementIdx].SlotIdx;
		Desc.AlignedByteOffset = APIVertexElements[VertexElementIdx].Offset;
		Desc.InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		Desc.InstanceDataStepRate = 0;

		DescArray.push_back(Desc);
	}

	VertexDeclarationD3D10* pVertexDeclaration = new VertexDeclarationD3D10;

	HRESULT HR = _pDevice_->CreateInputLayout(&(DescArray[0]), VertexElementCount, pAPIVertexShader->Code, pAPIVertexShader->CodeLength, &(pVertexDeclaration->pInputLayout));
	
	if (FAILED(HR))
	{
		delete pVertexDeclaration;

		return nullptr;
	}
	else
	{
		return pVertexDeclaration;
	}

}

void RenderSystemD3D10::SetVertexDeclaration(const VertexDeclaration* pVertexDeclaration)
{
	const VertexDeclarationD3D10* pAPIVertexDeclaration = static_cast<const VertexDeclarationD3D10*>(pVertexDeclaration);

	_pDevice_->IASetInputLayout(pAPIVertexDeclaration->pInputLayout);

}

// TODO: CPU access flags.
VertexBuffer* RenderSystemD3D10::CreateVertexBuffer(U32 Size, EResourceUsage Usage, const void* Data)
{
	VertexBufferD3D10* pVertexBuffer = new VertexBufferD3D10;

	D3D10_BUFFER_DESC Desc;
	Desc.ByteWidth = Size;
	Desc.Usage = static_cast<D3D10_USAGE>(_APIResourceUsageFlags_[Usage]);
	Desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA SubresourceData;
	SubresourceData.pSysMem = Data;
	SubresourceData.SysMemPitch = 0;
	SubresourceData.SysMemSlicePitch = 0;

	HRESULT HR = _pDevice_->CreateBuffer(&Desc, &SubresourceData, &(pVertexBuffer->pBuffer));
	
	if (FAILED(HR))
	{
		delete pVertexBuffer;

		return nullptr;
	}
	else
	{
		return pVertexBuffer;
	}
}

void RenderSystemD3D10::SetVertexBuffer(const VertexBuffer* pVertexBuffer, U32 SlotIdx, U32 Stride, U32 Offset)
{
	const VertexBufferD3D10* pAPIVertexBuffer = static_cast<const VertexBufferD3D10*>(pVertexBuffer);

	_pDevice_->IASetVertexBuffers(SlotIdx, 1, &(pAPIVertexBuffer->pBuffer), &Stride, &Offset);
}

// TODO: CPU acess flags.
UniformBuffer* RenderSystemD3D10::CreateUniformBuffer(U32 Size, EResourceUsage Usage, const void* Data)
{
	UniformBufferD3D10* pUniformBuffer = new UniformBufferD3D10;

	D3D10_BUFFER_DESC Desc;
	Desc.ByteWidth = Size;
	Desc.Usage = static_cast<D3D10_USAGE>(_APIResourceUsageFlags_[Usage]);
	Desc.BindFlags = D3D10_BIND_CONSTANT_BUFFER; 
	Desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA SubresourceData;
	SubresourceData.pSysMem = Data;
	SubresourceData.SysMemPitch = 0;
	SubresourceData.SysMemSlicePitch = 0;
	
	HRESULT HR = _pDevice_->CreateBuffer(&Desc, &SubresourceData, &(pUniformBuffer->pBuffer));

	if (FAILED(HR))
	{
		delete pUniformBuffer;

		return nullptr;
	}
	else
	{
		return pUniformBuffer;
	}
}

VertexShader* RenderSystemD3D10::CreateVertexShader(const void* Code, size_t Length)
{
	ID3D10Blob* pBlob;

	D3DCompile(Code, Length, NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Main", "vs_4_0", D3DCOMPILE_OPTIMIZATION_LEVEL0, 0, &pBlob, NULL);

	VertexShaderD3D10* pVertexShader = new VertexShaderD3D10;
	
	pVertexShader->Code = pBlob->GetBufferPointer();
	pVertexShader->CodeLength = pBlob->GetBufferSize();

	HRESULT HR = _pDevice_->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &(pVertexShader->pShader));
	
	if (FAILED(HR))
	{
		delete pVertexShader;

		return nullptr;
	}
	else
	{
		return pVertexShader;
	}
}

void RenderSystemD3D10::SetShaderUniformBuffer(const VertexShader* pVertexShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer)
{
	const UniformBufferD3D10* pAPIUniformBuffer = static_cast<const UniformBufferD3D10*>(pUniformBuffer);

	_pDevice_->VSSetConstantBuffers(SlotIdx, 1, &(pAPIUniformBuffer->pBuffer));
}

void RenderSystemD3D10::SetShaderUniformBuffer(const PixelShader* pPixelShader, U32 SlotIdx, const UniformBuffer* pUniformBuffer)
{
	const UniformBufferD3D10* pAPIUniformBuffer = static_cast<const UniformBufferD3D10*>(pUniformBuffer);

	_pDevice_->PSSetConstantBuffers(SlotIdx, 1, &(pAPIUniformBuffer->pBuffer));
}

PixelShader* RenderSystemD3D10::CreatePixelShader(const void* Code, size_t Length)
{
	ID3D10Blob* pBlob;

	D3DCompile(Code, Length, NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Main", "ps_4_0", D3DCOMPILE_OPTIMIZATION_LEVEL0, 0, &pBlob, NULL);

	PixelShaderD3D10* pPixelShader = new PixelShaderD3D10;
	
	HRESULT HR = _pDevice_->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &(pPixelShader->pShader));

	if (FAILED(HR))
	{
		delete pPixelShader;

		return nullptr;
	}
	else
	{
		return pPixelShader;
	}
}

RasterizerState* RenderSystemD3D10::CreateRasterizerState
(
	ERasterizerCullMode CullMode,														
	ERasterizerFillMode FillMode, 
	bool bEnableScissorTest, 
	bool bEnableLineAA, 
	bool bEnableMSAA,
	S32 DepthBias,
	F32 DepthBiasSlopeScale
)
{
	RasterizerStateD3D10* pRasterizerState = new RasterizerStateD3D10;

	D3D10_RASTERIZER_DESC Desc;
	Desc.AntialiasedLineEnable = static_cast<BOOL>(bEnableLineAA);
	Desc.MultisampleEnable = static_cast<BOOL>(bEnableMSAA);
	Desc.CullMode = static_cast<D3D10_CULL_MODE>(_APIRasterizerCullModes_[CullMode]);
	Desc.FillMode = static_cast<D3D10_FILL_MODE>(_APIRasterizerFillModes_[FillMode]);
	Desc.ScissorEnable = static_cast<BOOL>(bEnableScissorTest);
	Desc.FrontCounterClockwise = FALSE;
	Desc.DepthClipEnable = TRUE;
	Desc.DepthBiasClamp = 0.0f;
	Desc.DepthBias = DepthBias;
	Desc.SlopeScaledDepthBias = DepthBiasSlopeScale;

	HRESULT HR = _pDevice_->CreateRasterizerState(&Desc, &(pRasterizerState->pRasterizerState));

	if (FAILED(HR))
	{
		delete pRasterizerState;

		return nullptr;
	}
	else
	{
		return pRasterizerState;
	}
}

// TODO: bind flags, binding as shader resource is not always necessary
RenderTarget* RenderSystemD3D10::CreateRenderTarget(U32 Width, U32 Height, EGraphicsDataFormat Format)
{
	RenderTargetD3D10* pRenderTarget = new RenderTargetD3D10;
	
	D3D10_TEXTURE2D_DESC Texture2DDesc;
	Texture2DDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
	Texture2DDesc.Width = Width;
	Texture2DDesc.Height = Height;
	Texture2DDesc.Format = static_cast<DXGI_FORMAT>(_APIGraphicsDataFormats_[Format]);
	
	_pDevice_->CreateTexture2D(&Texture2DDesc, NULL, &(pRenderTarget->pTexture));
	
	D3D10_RENDER_TARGET_VIEW_DESC RTVDesc;
	RTVDesc.Format = static_cast<DXGI_FORMAT>(_APIGraphicsDataFormats_[Format]);
	RTVDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;
	
	HRESULT HR = _pDevice_->CreateRenderTargetView(pRenderTarget->pTexture, &RTVDesc, &(pRenderTarget->pRenderTargetBinding));

	if (FAILED(HR))
	{
		delete pRenderTarget;

		return nullptr;
	}
	else
	{
		return pRenderTarget;
	}
}

DepthStencilTarget* RenderSystemD3D10::CreateDepthStencilTarget(U32 Width, U32 Height, EGraphicsDataFormat Format)
{
	DepthStencilTargetD3D10* pDepthStencilTarget = new DepthStencilTargetD3D10;

	D3D10_TEXTURE2D_DESC Texture2DDesc;
	Texture2DDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	Texture2DDesc.Width = Width;
	Texture2DDesc.Height = Height;
	Texture2DDesc.Format = static_cast<DXGI_FORMAT>(_APIGraphicsDataFormats_[Format]);
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.CPUAccessFlags = 0;
	Texture2DDesc.Usage = D3D10_USAGE_DEFAULT;
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.MipLevels = 1;
	Texture2DDesc.MiscFlags = 0;

	HRESULT HR0 = _pDevice_->CreateTexture2D(&Texture2DDesc, NULL, &(pDepthStencilTarget->pTexture));

	if (FAILED(HR0))
	{
		delete pDepthStencilTarget;

		return nullptr;
	}

	D3D10_DEPTH_STENCIL_VIEW_DESC DSVDesc;
	DSVDesc.Format = static_cast<DXGI_FORMAT>(_APIGraphicsDataFormats_[Format]);
	DSVDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	HRESULT HR1 = _pDevice_->CreateDepthStencilView(pDepthStencilTarget->pTexture, &DSVDesc, &(pDepthStencilTarget->pDepthStencilTargetBinding));
	
	if (FAILED(HR1))
	{
		delete pDepthStencilTarget;

		return nullptr;
	}
	else
	{ 
		return pDepthStencilTarget;
	}
}

DepthStencilState* RenderSystemD3D10::CreateDepthStencilState
(
	bool bEnableDepthTest, 
	EComparisonFunction DepthComparisonFunc, 
	bool bEnableDepthMask, 
	bool bEnableStencilTest,
	U8 StencilReadMask,
	U8 StencilWriteMask,
	EStencilMode StencilFrontFailMode,
	EStencilMode DepthFrontFailMode,
	EStencilMode StencilFrontPassMode,
	EComparisonFunction StencilFrontComparisonFunc,
	EStencilMode StencilBackFailMode,
	EStencilMode DepthBackFailMode,
	EStencilMode StencilBackPassMode,
	EComparisonFunction StencilBackComparisonFunc
)
{
	DepthStencilStateD3D10* pDepthStencilState = new DepthStencilStateD3D10;
	
	D3D10_DEPTH_STENCIL_DESC Desc;
	Desc.DepthEnable = bEnableDepthTest;
	Desc.DepthFunc = static_cast<D3D10_COMPARISON_FUNC>(_APIComparisonFunctions_[DepthComparisonFunc]);
	Desc.DepthWriteMask = bEnableDepthMask ? D3D10_DEPTH_WRITE_MASK_ALL : D3D10_DEPTH_WRITE_MASK_ZERO;
	Desc.StencilEnable = bEnableStencilTest;
	Desc.StencilReadMask = StencilReadMask;
	Desc.StencilWriteMask = StencilWriteMask;
	Desc.FrontFace.StencilFailOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[StencilFrontFailMode]);
	Desc.FrontFace.StencilDepthFailOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[DepthFrontFailMode]);
	Desc.FrontFace.StencilPassOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[StencilFrontPassMode]);
	Desc.FrontFace.StencilFunc = static_cast<D3D10_COMPARISON_FUNC>(_APIComparisonFunctions_[StencilFrontComparisonFunc]);;
	Desc.BackFace.StencilFailOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[StencilBackFailMode]);
	Desc.BackFace.StencilDepthFailOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[DepthBackFailMode]);
	Desc.BackFace.StencilPassOp = static_cast<D3D10_STENCIL_OP>(_APIStencilModes_[StencilBackPassMode]);
	Desc.BackFace.StencilFunc = static_cast<D3D10_COMPARISON_FUNC>(_APIComparisonFunctions_[StencilBackComparisonFunc]);;
	
	HRESULT HR = _pDevice_->CreateDepthStencilState(&Desc, &(pDepthStencilState->pDepthStencilState));

	if (FAILED(HR))
	{
		delete pDepthStencilState;

		return nullptr;
	}
	else
	{
		return pDepthStencilState;
	}
}

BlendState* RenderSystemD3D10::CreateBlendState
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
)
{
	BlendStateD3D10* pBlendState = new BlendStateD3D10;

	D3D10_BLEND_DESC Desc;
	
	Desc.BlendEnable[0] = static_cast<BOOL>(bEnableBlend);
	Desc.AlphaToCoverageEnable = static_cast<BOOL>(bEnableAlphaToCoverage);
	Desc.SrcBlend = static_cast<D3D10_BLEND>(_APIBlendFactors_[SourceFactorRGB]);
	Desc.DestBlend = static_cast<D3D10_BLEND>(_APIBlendFactors_[DestinationFactorRGB]);
	Desc.SrcBlendAlpha = static_cast<D3D10_BLEND>(_APIBlendFactors_[SourceFactorAlpha]);
	Desc.DestBlendAlpha = static_cast<D3D10_BLEND>(_APIBlendFactors_[DestinationFactorAlpha]);
	Desc.BlendOp = static_cast<D3D10_BLEND_OP>(_APIBlendModes_[BlendModeRGB]);
	Desc.BlendOpAlpha = static_cast<D3D10_BLEND_OP>(_APIBlendModes_[BlendModeAlpha]);
	Desc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	
	HRESULT HR = _pDevice_->CreateBlendState(&Desc, &(pBlendState->pBlendState));

	if (FAILED(HR))
	{
		delete pBlendState;

		return nullptr;
	}
	else
	{
		return pBlendState;
	}
}

void RenderSystemD3D10::SetVertexShader(const VertexShader* pVertexShader)
{
	const VertexShaderD3D10* pAPIVertexShader = static_cast<const VertexShaderD3D10*>(pVertexShader);
	_pDevice_->VSSetShader(pAPIVertexShader->pShader);
}

void RenderSystemD3D10::SetPixelShader(const PixelShader* pPixelShader)
{
	const PixelShaderD3D10* pAPIPixelShader = static_cast<const PixelShaderD3D10*>(pPixelShader);
	_pDevice_->PSSetShader(pAPIPixelShader->pShader); 
}


void RenderSystemD3D10::SetViewport(U32 StartX, U32 StartY, U32 Width, U32 Height, F32 ZNear, F32 ZFar)
{
	D3D10_VIEWPORT APIViewport;
	APIViewport.TopLeftX = StartX;
	APIViewport.TopLeftY = StartY;
	APIViewport.Width = Width;
	APIViewport.Height = Height;
	APIViewport.MinDepth = ZNear;
	APIViewport.MaxDepth = ZFar;

	_pDevice_->RSSetViewports(1, &APIViewport);
}

void RenderSystemD3D10::SetRasterizerState(const RasterizerState* pRasterizerState)
{
	const RasterizerStateD3D10* pAPIRasterizerState = static_cast<const RasterizerStateD3D10*>(pRasterizerState);
	_pDevice_->RSSetState(pAPIRasterizerState->pRasterizerState);

}

void RenderSystemD3D10::SetDepthStencilState(const DepthStencilState* pDepthStencilState, U32 StencilRef)
{
	const DepthStencilStateD3D10* pAPIDepthStencilState = static_cast<const DepthStencilStateD3D10*>(pDepthStencilState);
	_pDevice_->OMSetDepthStencilState(pAPIDepthStencilState->pDepthStencilState, StencilRef);
}


void RenderSystemD3D10::SetBlendState(const BlendState* pBlendState, U32 SampleMask)
{
	F32 BlendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	const BlendStateD3D10* pAPIBlendState = static_cast<const BlendStateD3D10*>(pBlendState);
	_pDevice_->OMSetBlendState(pAPIBlendState->pBlendState, BlendFactor, SampleMask);
}


void RenderSystemD3D10::SetRenderTarget(const RenderTarget* pRenderTarget, const DepthStencilTarget* pDepthStencilTarget)
{
	const RenderTargetD3D10* pAPIRenderTarget = static_cast<const RenderTargetD3D10*>(pRenderTarget);
	const DepthStencilTargetD3D10* pAPIDepthStencilTarget = static_cast<const DepthStencilTargetD3D10*>(pDepthStencilTarget);

	_pDevice_->OMSetRenderTargets(pAPIRenderTarget ? 1 : 0,
								  pAPIRenderTarget ? &(pAPIRenderTarget->pRenderTargetBinding) : NULL, 
								  pAPIDepthStencilTarget ? pAPIDepthStencilTarget->pDepthStencilTargetBinding : NULL);
}

void RenderSystemD3D10::Draw(EPrimitiveType PrimitiveType, U32 VertexCount, U32 StartVertexLocation)
{
	_pDevice_->IASetPrimitiveTopology(static_cast<D3D10_PRIMITIVE_TOPOLOGY>(_APIPrimitiveTypes_[PrimitiveType]));

	_pDevice_->Draw(VertexCount, StartVertexLocation);
}

void RenderSystemD3D10::ClearRenderTarget(const RenderTarget* pRenderTarget, const Math::ColorRGBA& ClearColor)
{
	const RenderTargetD3D10* pAPIRenderTarget = static_cast<const RenderTargetD3D10*>(pRenderTarget);
	_pDevice_->ClearRenderTargetView(pAPIRenderTarget->pRenderTargetBinding, ClearColor.Elements);
}

void RenderSystemD3D10::ClearDepthStencilTarget(const DepthStencilTarget* pDepthStencilTarget, F32 DepthClearColor, U8 StencilClearColor)
{
	const DepthStencilTargetD3D10* pAPIDepthStencilTarget = static_cast<const DepthStencilTargetD3D10*>(pDepthStencilTarget);
	_pDevice_->ClearDepthStencilView(pAPIDepthStencilTarget->pDepthStencilTargetBinding, D3D10_CLEAR_DEPTH, DepthClearColor, StencilClearColor);
}
