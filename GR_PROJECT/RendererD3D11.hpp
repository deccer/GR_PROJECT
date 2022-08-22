#pragma once

#include "Renderer.hpp"

#include <dxgi.h>
#include <d3d11.h>

struct PipelineD3D11 {
	ID3D11DeviceChild* shader;
	GR_PIPELINE_TYPE pipelineType;
};

class RendererD3D11 : public Renderer
{
private:
	IDXGIFactory* _factory = nullptr;
	IDXGIAdapter* _adapter = nullptr;
	IDXGISwapChain* _swapchain = nullptr;

	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _context = nullptr;

#ifdef _DEBUG || DEBUG
	ID3D11Debug* _debug = nullptr;
	ID3D11InfoQueue* _debugInfoQueue = nullptr;
#endif

	ID3D11DepthStencilView* _depthStencilBufferDSV = nullptr;
	ID3D11RenderTargetView* _backBufferRTV = nullptr;
	ID3D11RenderTargetView* _materialIdBufferRTV = nullptr;
	ID3D11ShaderResourceView* _materialIdBufferSRV = nullptr;
	ID3D11RenderTargetView* _colorMetalnessRoughnessBufferRTV = nullptr;
	ID3D11ShaderResourceView* _colorMetalnessRoughnessBufferSRV = nullptr;
	ID3D11RenderTargetView* _normalsTangentBufferRTV = nullptr;
	ID3D11ShaderResourceView* _normalsTangentBufferSRV = nullptr;

	std::vector<PipelineD3D11> _pipelines;

	bool InitDXGI();
	bool InitAdapter();
	bool InitDirect3D();
	bool InitSwapchain();
	bool InitGBuffers();
	bool InitConstState();

public:
	RendererD3D11(GLFWwindow* window)
		: Renderer(window) {}

	bool Init() override;
	void Shutdown() override;

	void Draw() override;
};