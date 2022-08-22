#include "RendererD3D11.hpp"

bool RendererD3D11::InitDXGI() 
{
	if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&_factory))))
		return false;

	return true;
}

bool RendererD3D11::InitAdapter() 
{
	IDXGIAdapter* tempAdapter;
	std::vector<IDXGIAdapter*> adapters;
	while (SUCCEEDED(_factory->EnumAdapters(adapters.size(), &tempAdapter)))
		adapters.push_back(tempAdapter);

	if (adapters.empty())
		return false;

	std::cout << "Avaliable adapters:\n";
	for (size_t i = 0; i < adapters.size(); ++i) {
		DXGI_ADAPTER_DESC adapterDesc;
		if (FAILED(adapters[i]->GetDesc(&adapterDesc)))
			return false;
		std::wcout << i << ") " << adapterDesc.Description << "\n";
	}
	std::cout << "\n";

	size_t selection = 0;
	/*std::cout << "Please select an adapter: ";
	std::cin >> selection;
	while (selection >= adapters.size()) {
		std::cout << "\nPlease make a valid selection.\n";
		std::cout << "Please select an adapter: ";
		std::cin >> selection;
	}
	std::cout << "\n";*/

	_adapter = adapters[selection];

	for (IDXGIAdapter* adapter : adapters)
		if (adapter != _adapter)
			adapter->Release();

	return true;
}

bool RendererD3D11::InitDirect3D() 
{
	uint32_t flags = 0;

#ifdef _DEBUG || DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL returnFeatureLevel;
	const D3D_FEATURE_LEVEL desiredFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	if (FAILED(D3D11CreateDevice(_adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, flags, &desiredFeatureLevel, 1, D3D11_SDK_VERSION, &_device, &returnFeatureLevel, &_context)))
		return false;

	if (returnFeatureLevel < desiredFeatureLevel)
		return false;

#ifdef _DEBUG || DEBUG
	if (FAILED(_device->QueryInterface(IID_PPV_ARGS(&_debug))))
		return false;

	if (FAILED(_debug->QueryInterface(IID_PPV_ARGS(&_debugInfoQueue))))
		return false;
#endif

	return true;
}

bool RendererD3D11::InitSwapchain()
{
	int32_t width, height;
	glfwGetWindowSize(_window, &width, &height);

	DXGI_SWAP_CHAIN_DESC swapchainDesc = {
		.BufferDesc = {
			.Width = static_cast<uint32_t>(width),
			.Height = static_cast<uint32_t>(height),
			.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
			.Scaling = DXGI_MODE_SCALING_STRETCHED
		},
		.SampleDesc = {
			.Count = 1
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 3,
		.OutputWindow = glfwGetWin32Window(_window),
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
		.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	if (FAILED(_factory->CreateSwapChain(_device, &swapchainDesc, &_swapchain)))
		return false;

	return true;
}

bool RendererD3D11::InitGBuffers()
{
	ID3D11Texture2D* swapchainBuffer;
	if (FAILED(_swapchain->GetBuffer(0, IID_PPV_ARGS(&swapchainBuffer))))
		return false;

	if (FAILED(_device->CreateRenderTargetView(swapchainBuffer, nullptr, &_backBufferRTV)))
		return false;

	swapchainBuffer->Release();

	int32_t width, height;
	glfwGetWindowSize(_window, &width, &height);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {
		.Width = static_cast<uint32_t>(width),
		.Height = static_cast<uint32_t>(height),
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = {
			.Count = 1
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL
	};

	ID3D11Texture2D* depthStencilBuffer;
	if (FAILED(_device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &depthStencilBuffer)))
		return false;

	if (FAILED(_device->CreateDepthStencilView(depthStencilBuffer, nullptr, &_depthStencilBufferDSV)))
		return false;

	depthStencilBuffer->Release();

	D3D11_TEXTURE2D_DESC materialIdBufferDesc = {
		.Width = static_cast<uint32_t>(width),
		.Height = static_cast<uint32_t>(height),
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R16_UINT,
		.SampleDesc = {
			.Count = 1
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	};

	ID3D11Texture2D* materialIdBuffer;
	if (FAILED(_device->CreateTexture2D(&materialIdBufferDesc, nullptr, &materialIdBuffer)))
		return false;

	if (FAILED(_device->CreateRenderTargetView(materialIdBuffer, nullptr, &_materialIdBufferRTV)))
		return false;

	if (FAILED(_device->CreateShaderResourceView(materialIdBuffer, nullptr, &_materialIdBufferSRV)))
		return false;

	materialIdBuffer->Release();

	D3D11_TEXTURE2D_DESC colorMetalnessRoughnessBufferDesc = {
		.Width = static_cast<uint32_t>(width),
		.Height = static_cast<uint32_t>(height),
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R8G8B8A8_UINT,
		.SampleDesc = {
			.Count = 1
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	};

	ID3D11Texture2D* colorMetalnessRoughnessBuffer;
	if (FAILED(_device->CreateTexture2D(&colorMetalnessRoughnessBufferDesc, nullptr, &colorMetalnessRoughnessBuffer)))
		return false;

	if (FAILED(_device->CreateRenderTargetView(colorMetalnessRoughnessBuffer, nullptr, &_colorMetalnessRoughnessBufferRTV)))
		return false;

	if (FAILED(_device->CreateShaderResourceView(colorMetalnessRoughnessBuffer, nullptr, &_colorMetalnessRoughnessBufferSRV)))
		return false;

	colorMetalnessRoughnessBuffer->Release();

	D3D11_TEXTURE2D_DESC normalsTangentBufferDesc = {
		.Width = static_cast<uint32_t>(width),
		.Height = static_cast<uint32_t>(height),
		.ArraySize = 1,
		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
		.SampleDesc = {
			.Count = 1
		},
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
	};

	ID3D11Texture2D* normalsTangentBuffer;
	if (FAILED(_device->CreateTexture2D(&normalsTangentBufferDesc, nullptr, &normalsTangentBuffer)))
		return false;

	if (FAILED(_device->CreateRenderTargetView(normalsTangentBuffer, nullptr, &_normalsTangentBufferRTV)))
		return false;

	if (FAILED(_device->CreateShaderResourceView(normalsTangentBuffer, nullptr, &_normalsTangentBufferSRV)))
		return false;

	normalsTangentBuffer->Release();

	return true;
}

bool RendererD3D11::InitConstState()
{
	return true;
}

bool RendererD3D11::Init()
{
	if (!InitDXGI())
		return false;
	if (!InitAdapter())
		return false;
	if (!InitDirect3D())
		return false;
	if (!InitSwapchain())
		return false;
	if (!InitGBuffers())
		return false;
	if (!InitConstState())
		return false;

	return true;
}

void RendererD3D11::Shutdown()
{
	_normalsTangentBufferSRV->Release();
	_normalsTangentBufferRTV->Release();
	_colorMetalnessRoughnessBufferSRV->Release();
	_colorMetalnessRoughnessBufferRTV->Release();
	_materialIdBufferSRV->Release();
	_materialIdBufferRTV->Release();
	_backBufferRTV->Release();
	_depthStencilBufferDSV->Release();

	_swapchain->Release();

	_debugInfoQueue->Release();
	_debug->Release();

	_context->Release();
	_device->Release();

	_adapter->Release();

	_factory->Release();
}

void RendererD3D11::Draw()
{
	_context->OMSetRenderTargets(1, &_backBufferRTV, _depthStencilBufferDSV);

	const float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
	_context->ClearRenderTargetView(_backBufferRTV, clearColor);

	_swapchain->Present(0, 0);
}