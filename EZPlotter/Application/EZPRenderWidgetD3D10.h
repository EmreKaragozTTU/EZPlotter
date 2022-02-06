
#pragma once

#include "QtWidgets\qwidget.h"
#include "Renderer\Backend\D3D10\RenderSystemD3D10.h"
#include "Renderer\Plot3DRenderer.h"


#include <dxgi.h>
#include <d3d10.h>

class EZPMainConsole;

enum EPlottingMode
{
	PLOT_2D,
	PLOT_3D
};

class EZPRenderWidgetD3D10 : public QWidget 
{
	// Q_OBJECT
  // Q_DISABLE_COPY(EZPRenderWidgetD3D10)

	public:

		EZPRenderWidgetD3D10(QWidget* parent = NULL);
		virtual ~EZPRenderWidgetD3D10();
		
		virtual QPaintEngine* paintEngine() const { return NULL; }

		RenderTargetD3D10* GetBackBufferPtr() { return &_BackBuffer_; };

		RenderSystemInterface* GetRenderSystemInterfacePtr() { return _RenderSystemInterfacePtr_; };

		void SaveFramebufferAsImage(const char* ImageFileName, const char* ImageFileFormat);

		void SetPlotID(U32 PlotID) { _PlotID_ = PlotID; };

		void SetPlottingMode(EPlottingMode Mode) { _PlottingMode_ = Mode; };

		void SetPlot3DRenderData(Plot3DRenderData* pRenderData) { _pPlot3DRenderData_ = pRenderData; };
		void SetPlot3DRenderState(Plot3DRenderState* pRenderState) { _pPlot3DRenderState_ = pRenderState; };

	protected:

		//virtual void resizeEvent(QResizeEvent* Event);
		virtual void paintEvent(QPaintEvent* Event);
		virtual void closeEvent(QCloseEvent* Event);
		//virtual void wheelEvent(QWheelEvent* Event);
		//virtual void mousePressEvent(QMouseEvent* Event);
		//virtual void mouseReleaseEvent(QMouseEvent* Event);
		//virtual void mouseMoveEvent(QMouseEvent* Event);
		virtual void keyPressEvent(QKeyEvent* Event);

	private:
		
		U32 _PlotID_;

		ID3D10Device* _pDevice_;
		IDXGISwapChain* _pSwapChain_;
		DXGI_SWAP_CHAIN_DESC _SwapChainDesc_;
		RenderTargetD3D10 _BackBuffer_;
		RenderSystemInterface* _RenderSystemInterfacePtr_;
		IDXGIAdapter* adapter_;

		// TODO: Add member RenderData
		Plot3DRenderData* _pPlot3DRenderData_;
		Plot3DRenderState* _pPlot3DRenderState_;
		
		// TODO: Add member RenderState

		Math::Vector2 _MousePosPrev_;
		F32 _CameraAngleX_;
		F32 _CameraAngleY_;

		F32 _RotationAngleX_;
		F32 _RotationAngleY_;

		EPlottingMode _PlottingMode_;

  
};
