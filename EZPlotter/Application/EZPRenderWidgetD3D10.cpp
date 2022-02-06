
#include "EZPRenderWidgetD3D10.h"
#include "QtGui\qevent.h"
#include "QtCore\qtimer.h"
#include "QtGui\qimagewriter.h"
#include "QtGui\qimage.h"
#include "EZPMainConsole.h"

#include <cassert>
#include <sstream>
#include <fstream>

EZPRenderWidgetD3D10::EZPRenderWidgetD3D10(QWidget* parent) : QWidget(parent) 
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	setAttribute(Qt::WA_DeleteOnClose, true);
	
	setMinimumSize(480, 360);
	setMaximumSize(960, 720);

	resize(480, 360);

	_SwapChainDesc_.BufferDesc.Width = width();
	_SwapChainDesc_.BufferDesc.Height = height();
	_SwapChainDesc_.BufferDesc.RefreshRate.Numerator = 60;
	_SwapChainDesc_.BufferDesc.RefreshRate.Denominator = 1;
	_SwapChainDesc_.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_SwapChainDesc_.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	_SwapChainDesc_.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	_SwapChainDesc_.SampleDesc.Count = 8;
	_SwapChainDesc_.SampleDesc.Quality = 0;
	_SwapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_SwapChainDesc_.BufferCount = 1;
	_SwapChainDesc_.OutputWindow = (HWND)winId();
	_SwapChainDesc_.Windowed = TRUE;
	_SwapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	_SwapChainDesc_.Flags = 0;

	
	D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &_SwapChainDesc_, &_pSwapChain_, &_pDevice_);

	_RenderSystemInterfacePtr_ = new RenderSystemD3D10(_pDevice_);
	
	_pSwapChain_->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&(_BackBuffer_.pTexture));

	_pDevice_->CreateRenderTargetView(_BackBuffer_.pTexture, 0, &(_BackBuffer_.pRenderTargetBinding));


	_BackBuffer_.Width = width();
	_BackBuffer_.Height = height();

	//_RenderSystemInterfacePtr_->ClearRenderTarget(&_BackBuffer_, Math::ColorRGBA{ 1.0f, 1.0f, 1.0f, 1.0f });

	//_RenderSystemInterfacePtr_->SetRenderTarget(&_BackBuffer_, nullptr);

	_RenderSystemInterfacePtr_->SetViewport(0, 0, width(), height(), 0.0f, 1.0f);

	_RotationAngleX_ = 0;
	_RotationAngleY_ = 0;

	//QTimer* timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	//timer->start(16);
}

EZPRenderWidgetD3D10::~EZPRenderWidgetD3D10() 
{
	_pSwapChain_->Release();
}
void EZPRenderWidgetD3D10::closeEvent(QCloseEvent* Event)
{
	EZPMainConsole::getInstance()->RenderWidgetArray[_PlotID_] = nullptr;
}

/*
void EZPRenderWidgetD3D10::resizeEvent(QResizeEvent* Event) 
{
	_pSwapChain_->ResizeBuffers(1, width(), height(), swapChainDesc_.BufferDesc.Format, 0);
	swapChain_->GetDesc(&swapChainDesc_);
	viewport_.Width = width();
	viewport_.Height = height();
	
	//createBuffers();
}
*/


void EZPRenderWidgetD3D10::paintEvent(QPaintEvent* Event) 
{
	_pSwapChain_->Present(0, 0);
}


void EZPRenderWidgetD3D10::SaveFramebufferAsImage(const char* ImageFileName, const char* ImageFileFormat)
{
	U32 Width = _BackBuffer_.Width;
	U32 Height = _BackBuffer_.Height;

	D3D10_TEXTURE2D_DESC Desc;
	Desc.Width = Width;
	Desc.Height = Height;
	Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Desc.ArraySize = 1;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.BindFlags = 0;
	Desc.MipLevels = 1;
	Desc.MiscFlags = 0;
	Desc.Usage = D3D10_USAGE_STAGING;
	Desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;

	ID3D10Texture2D *pTexture;
	if (SUCCEEDED(_pDevice_->CreateTexture2D(&Desc, NULL, &pTexture)))
	{	
		if (_SwapChainDesc_.SampleDesc.Count > 1)
		{
			ID3D10Texture2D *resolved = NULL;
			Desc.Usage = D3D10_USAGE_DEFAULT;
			Desc.CPUAccessFlags = 0;

			if (SUCCEEDED(_pDevice_->CreateTexture2D(&Desc, NULL, &resolved)))
			{
				_pDevice_->ResolveSubresource(resolved, 0, _BackBuffer_.pTexture, 0, Desc.Format);
				_pDevice_->CopyResource(pTexture, resolved);
				resolved->Release();
			}
		}
		
		else
		{
			_pDevice_->CopyResource(pTexture, _BackBuffer_.pTexture);
		}
		
		D3D10_MAPPED_TEXTURE2D MappedTexture; 
		if (SUCCEEDED(pTexture->Map(0, D3D10_MAP_READ, 0, &MappedTexture)))
		{

			U8* Dst = new U8[Width * Height * 3];
			U8* DstStart = Dst;
			//std::vector<U8> Dst;
			//Dst.reserve(Width * Height * 3);
			
			U8* Src = (U8*)MappedTexture.pData;

			for (int IdxY = 0; IdxY < Height; IdxY++)
			{
				for (int IdxX = 0; IdxX < Width; IdxX++)
				{
					Dst[3 * IdxX + 0] = Src[4 * IdxX + 0];
					Dst[3 * IdxX + 1] = Src[4 * IdxX + 1];
					Dst[3 * IdxX + 2] = Src[4 * IdxX + 2];
				}

				Dst += Width * 3;
				Src += MappedTexture.RowPitch;
			}
			
			pTexture->Unmap(0);
			
			QImage Image(DstStart, Width, Height, Width * 3, QImage::Format_RGB888);

			QImageWriter Writer(ImageFileName, ImageFileFormat);
			Writer.write(Image);

			delete[] DstStart;
		}

		pTexture->Release();
	}
}

/*
void EZPRenderWidgetD3D10::createBuffers() 
{
  HRESULT hr;

  hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), 
							 (LPVOID*)&backBuffer_);
  assert(!FAILED(hr));

  hr = device_->CreateRenderTargetView(backBuffer_, NULL, &rtView_);
  assert(!FAILED(hr));

  ZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));
  viewport_.MinDepth = 0.0f;
  viewport_.MaxDepth = 1.0f;
  viewport_.TopLeftX = 0;
  viewport_.TopLeftY = 0;
  viewport_.Width = width();
  viewport_.Height = height();


  ID3DBlob* vsBlob = NULL;
  ID3DBlob* psBlob = NULL;

  compileShader("test1.fx", "VSMain", "vs_4_0", &vsBlob);
  hr = device_->CreateVertexShader(vsBlob->GetBufferPointer(),
								   vsBlob->GetBufferSize(), NULL,
								   &vshader_);
  assert(!FAILED(hr));

  // Test reflection
  ID3D11ShaderReflection* refl;
  hr = D3DReflect(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
				  IID_ID3D11ShaderReflection, (LPVOID*)&refl);
  assert(!FAILED(hr));

  D3D11_SHADER_Desc shaderDesc;
  refl->GetDesc(&shaderDesc);
  

  compileShader("test1.fx", "PSMain", "ps_4_0", &psBlob);
  hr = device_->CreatePixelShader(psBlob->GetBufferPointer(),
								  psBlob->GetBufferSize(), NULL,
								  &pshader_);
  assert(!FAILED(hr));

  D3D11_INPUT_ELEMENT_Desc layout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
	 D3D11_INPUT_PER_VERTEX_DATA, 0},
  };
  UINT numElements = 1;

  hr = device_->CreateInputLayout(layout, numElements,
								  vsBlob->GetBufferPointer(),
								  vsBlob->GetBufferSize(), &layout_);
  assert(!FAILED(hr));

  struct Vertex {
	float x, y, z;
  };
  
  std::vector<Vertex> vertices;
  vertices.reserve(1024);
  
  const int width = 30;
  const int depth = 100;

  const float spacePerTile = 0.1f;

  float currX = 0.0f;
  float currZ = 0.0f;

  for(int i = 0; i < depth; ++i) {
	currZ = 0.0f;
	for(int j = 0; j < width; ++j) {
	  Vertex v;
	  v.x = currX;
	  v.y = 0.0f;
	  v.z = currZ;
	  vertices.push_back(v);
	  currZ += spacePerTile;
	}
	currX += spacePerTile;
  }

  D3D11_BUFFER_Desc Desc;
  ZeroMemory(&Desc, sizeof(D3D11_BUFFER_Desc));
  Desc.Usage = D3D11_USAGE_DEFAULT;
  Desc.ByteWidth = sizeof(Vertex) * vertices.size();
  Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  Desc.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA initData;
  ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
  initData.pSysMem = &vertices[0];

  hr = device_->CreateBuffer(&Desc, &initData, &vbuffer_);
  assert(!FAILED(hr));


  std::vector<WORD> indices;
  indices.reserve(1024);

  WORD baseIndex = 0;

  for(int i = 0; i < depth-1; ++i) {
	for(int j = 0; j < width-1; ++j) {
	  indices.push_back(baseIndex+j);
	  indices.push_back(baseIndex+j+1);
	  indices.push_back(baseIndex+j+width+1);
	  indices.push_back(baseIndex+j+width+1);
	  indices.push_back(baseIndex+j+width);
	  indices.push_back(baseIndex+j);
	}
	baseIndex += width;
  }

  indexCount = indices.size();
  

  ZeroMemory(&Desc, sizeof(D3D11_BUFFER_Desc));

  Desc.Usage = D3D11_USAGE_DEFAULT;
  Desc.ByteWidth = sizeof(WORD) * indices.size();
  Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  Desc.CPUAccessFlags = 0;

  ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
  initData.pSysMem = &indices[0];

  hr = device_->CreateBuffer(&Desc, &initData, &ibuffer_);
  assert(!FAILED(hr));


  ZeroMemory(&Desc, sizeof(D3D11_BUFFER_Desc));
  Desc.Usage = D3D11_USAGE_DYNAMIC;
  Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  Desc.MiscFlags = 0;
  Desc.ByteWidth = sizeof(Matrix44); // One matrix
  hr = device_->CreateBuffer(&Desc, NULL, &cbPerObject_);
  assert(!FAILED(hr));


  ZeroMemory(&Desc, sizeof(D3D11_BUFFER_Desc));
  Desc.Usage = D3D11_USAGE_DYNAMIC;
  Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  Desc.MiscFlags = 0;
  Desc.ByteWidth = sizeof(float)*4; // One float
  hr = device_->CreateBuffer(&Desc, NULL, &cb2_);
  assert(!FAILED(hr));
}
*/

/*
void EZPRenderWidgetD3D10::render() 
{
  float clearColor[] = {0.3f, 0.3f, 0.3f, 1.0f};

  

  context_->OMSetRenderTargets(1, &rtView_, NULL);
  context_->RSSetViewports(1, &viewport_);
  context_->ClearRenderTargetView(rtView_, clearColor);

  context_->IASetInputLayout(layout_);

  UINT stride = sizeof(float) * 3;
  UINT offset = 0;
  context_->IASetVertexBuffers(0, 1, &vbuffer_, &stride, &offset);
  context_->IASetIndexBuffer(ibuffer_, DXGI_FORMAT_R16_UINT, 0);
  context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  context_->VSSetShader(vshader_, NULL, 0);
  context_->PSSetShader(pshader_, NULL, 0);

  HRESULT hr;

  D3D11_MAPPED_SUBRESOURCE mappedRes;
  
  hr = context_->Map(cb2_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
  assert(!FAILED(hr));
  float* time = (float*)mappedRes.pData;
  *time = currTime;
  currTime += 0.1f;
  context_->Unmap(cb2_, 0);
  
  hr = context_->Map(cbPerObject_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
  assert(!FAILED(hr));

  Matrix44* worldViewProj = (Matrix44*)mappedRes.pData;

  Matrix44 proj, view;

  Vector3 position, target, up;

  position.x = 0.0f;
  position.y = 0.0f;
  position.z = 7.0f;
  target.x = 0.0f;
  target.y = 0.0f;
  target.z = 0.0f;
  up.x = 0.0f;
  up.y = 1.0f;
  up.z = 0.0f;

  position.x = std::sin(cameraAngleY_)*cameraDistance_;
  position.y = std::sin(cameraAngleX_)*cameraDistance_;
  position.z = std::cos(cameraAngleY_)*cameraDistance_;

  XMMATRIX xnaView = XMMatrixLookAtRH(
	XMVectorSet(position.x, position.y, position.z, 0.0f),
	XMVectorSet(target.x, target.y, target.z, 0.0f),
	XMVectorSet(up.x, up.y, up.z, 0.0f));

  XMMATRIX xnaProj = XMMatrixPerspectiveFovRH(0.523598775f, 
											  (float)width() / (float)height(),
											  1.0f, 1000.0f);                                      

  xnaView = XMMatrixTranspose(xnaView);
  xnaProj = XMMatrixTranspose(xnaProj);

  view.makeLookAt(position, target, up);
  proj.makeProjection(0.523598775f, (float)width() / (float)height(),
					  1.0f, 1000.0f);
  
  // Direct3D is row-major
  //view.transpose();
  //proj.transpose();
  //Matrix44 viewProj = view * proj;
  Matrix44 viewProj = proj * view;

  // D3D needs the matrices in column-major order
  viewProj.transpose();

  *worldViewProj = viewProj;

  context_->Unmap(cbPerObject_, 0);

  context_->VSSetConstantBuffers(0, 1, &cbPerObject_);
  context_->VSSetConstantBuffers(1, 1, &cb2_);

  ID3D11RasterizerState* state;
  D3D11_RASTERIZER_Desc Desc;
  ZeroMemory(&Desc, sizeof(D3D11_RASTERIZER_Desc));
  //Desc.FillMode = D3D11_FILL_SOLID;
  Desc.FillMode = D3D11_FILL_WIREFRAME;
  Desc.CullMode = D3D11_CULL_NONE;
  device_->CreateRasterizerState(&Desc, &state);
  context_->RSSetState(state);
  state->Release();

  //context_->Draw(6, 0);
  context_->DrawIndexed(indexCount, 0, 0);

  swapChain_->Present(0, 0);
}
*/

/*
void EZPRenderWidgetD3D10::wheelEvent(QWheelEvent* Event) 
{
	float delta      = (float)Event->delta();
	cameraDistance_ -= (delta / 120.0f / 4.0f);
	cameraDistance_  = std::max(1.0f, cameraDistance_);
	update();
}
*/


/*

void EZPRenderWidgetD3D10::mousePressEvent(QMouseEvent* Event) 
{
	if(Event->button() == Qt::LeftButton) 
	{
		_MousePosPrev_ = Math::Vector2{ static_cast<F32>(Event->pos().x()), static_cast<F32>(Event->pos().y()) };
	} 
	else
	{
		Event->ignore();
	}
}


void EZPRenderWidgetD3D10::mouseReleaseEvent(QMouseEvent* Event) 
{
	Event->ignore(); 
}

void EZPRenderWidgetD3D10::mouseMoveEvent(QMouseEvent* Event) 
{
	if(Event->buttons() & Qt::LeftButton) 
	{
		Math::Vector2 MousePosCurrent  = Event->posF();
		Math::Vector2 DeltaPos = curr - mousePrev_;
		mousePrev_    = curr;

		cameraAngleY_ -= (delta.x()) / 200.0f;
		cameraAngleX_ += (delta.y()) / 200.0f;

		cameraAngleX_ = std::min(1.57f, std::max(-1.57f, cameraAngleX_));
  
		update();
	} 
	else 
	{
		Event->ignore();
	}
}
*/

void EZPRenderWidgetD3D10::keyPressEvent(QKeyEvent* Event)
{
	if (_PlottingMode_ == EPlottingMode::PLOT_2D)
	{
		return;
	}

	switch (Event->key())
	{
		case Qt::Key_Up: _RotationAngleX_ += 5.0f; break;

		case Qt::Key_Down: _RotationAngleX_ -= 5.0f; break;

		case Qt::Key_Right: _RotationAngleY_ += 5.0f; break;

		case Qt::Key_Left: _RotationAngleY_ -= 5.0f; break;

		default: return;
	}

	// TODO: Clean this mess, move to another place.
	F32 OriginX = (_pPlot3DRenderData_->XMax + _pPlot3DRenderData_->XMin) / 2.0f;
	F32 OriginDistX = fabsf(_pPlot3DRenderData_->XMax - OriginX);

	F32 OriginY = (_pPlot3DRenderData_->YMax + _pPlot3DRenderData_->YMin) / 2.0f;
	F32 OriginDistY = fabsf(_pPlot3DRenderData_->YMax - OriginY);

	F32 OriginZ = (_pPlot3DRenderData_->ZMax + _pPlot3DRenderData_->ZMin) / 2.0f;
	F32 OriginDistZ = fabsf(_pPlot3DRenderData_->ZMax - OriginZ);

	Math::Matrix4x4 WorldMatrix = Math::Scale(1.0f / OriginDistX, 1.0f / OriginDistY, 1.0f / (OriginDistZ * 2.0f)) * Math::RotationX(_RotationAngleX_) * Math::RotationZ(_RotationAngleY_) * Math::Translation(0.0f, 0.0f, 0.0f);
	Math::Matrix4x4 ViewMatrix = Math::LookAtMatrixLH(Math::Vector3{ 2.0f, -2.0f, 3.0f }, Math::Vector3{ 0.0f, 0.0f, 0.0f }, Math::Vector3{ 0.0f, 0.0f, 1.0f });
	Math::Matrix4x4 ProjectionMatrix = Math::PerspectiveFovMatrixLH(80.0f, (F32)(_pPlot3DRenderState_->pRenderTarget->Width) / (F32)(_pPlot3DRenderState_->pRenderTarget->Height), 0.0f, 1024.0f);
	

	struct VSUniformBufferData
	{
		Math::Matrix4x4 WVPMatrix;
	};

	VSUniformBufferData SrcData;
	VSUniformBufferData* pDstData = NULL;

	SrcData.WVPMatrix = Math::Transpose(WorldMatrix * ViewMatrix * ProjectionMatrix);

	ID3D10Buffer* pBuffer = static_cast<UniformBufferD3D10*>(_pPlot3DRenderState_->pVSUniformBuffer)->pBuffer;

	HRESULT HR = pBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&pDstData);

	if (FAILED(HR))
	{
		qDebug("Resource Mapping Failed");

		return;
	}

	pDstData->WVPMatrix = SrcData.WVPMatrix;

	memcpy(pDstData, &SrcData, sizeof(VSUniformBufferData));

	pBuffer->Unmap();

	Plot3DRenderer::Render(_RenderSystemInterfacePtr_, _pPlot3DRenderState_);
	update();
}