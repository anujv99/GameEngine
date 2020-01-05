#include "directxcontext.h"

#ifdef ENGINE_DIRECTX

#if defined(ENGINE_DEBUG)
	#define CHECK_CONTEXT_CREATION(F) ASSERT(F == S_OK)
	#define CHECK_HR(F, M) ASSERTM(F == S_OK, M)
#else
	#define CHECK_CONTEXT_CREATION(F) F
	#define CHECK_HR(F, M) F
#endif

#include <comutil.h>

namespace prev {

	IFNOT_OPENGL(GraphicsAPI GraphicsContext::s_GraphicsAPI = GraphicsAPI::API_OPENGL);

	ComPtr<ID3D11Device> s_Device;
	ComPtr<ID3D11DeviceContext> s_DeviceContext;


	GraphicsContext * GraphicsContext::CreateDirectXContext() {
		return dynamic_cast<GraphicsContext *>(new DirectXContext());
	}

	DirectXContext::DirectXContext() : m_WindowPtr(nullptr), m_HWnd(nullptr) {}

	DirectXContext::~DirectXContext() {}

	void DirectXContext::BeginFrame() {
		PV_PROFILE_FUNCTION();
		float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DirectXContext::EndFrame() {
		PV_PROFILE_FUNCTION();
		m_SwapChain->Present(1u, 0u);
	}

	void DirectXContext::CreateContext(pvptr rawWindowPtr) {
		PV_PROFILE_FUNCTION();
		m_WindowPtr = reinterpret_cast<::GLFWwindow *>(rawWindowPtr);
		ASSERTM(m_WindowPtr, "Unable to create DirectX context without a valid window");

		m_HWnd = glfwGetWin32Window(m_WindowPtr);
		ASSERTM(m_HWnd, "Unable to create DirectX context without a valid window handle");

		int width = 0, height = 0;

		glfwGetWindowSize(m_WindowPtr, &width, &height);

		HRESULT hr = CoInitialize(nullptr);
		ASSERTM(hr == S_OK, "Unable to CoInitialize");

		DXGI_MODE_DESC bd;
		bd.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		bd.RefreshRate.Numerator	= 0;
		bd.RefreshRate.Denominator	= 0;
		bd.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		bd.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bd.Width					= width;
		bd.Height					= height;

		InitializeDirectX(bd);
		CreateRenderTargetView();
		CreateDepthBuffer(bd);
		CreateRasterizerState();

		LOG_INFO("[DirectX] Successfully created DirectX context");

		LogInfo();

		s_Device = m_Device;
		s_DeviceContext = m_DeviceContext;
	}

	void DirectXContext::ChangeResolution(UINT width, UINT height) {
		ComPtr<ID3D11Resource> backBuffer;
		HRESULT hr = m_SwapChain->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void **>(backBuffer.GetAddressOf()));
		ASSERTM(hr == S_OK, "[DirectX] Failed to get back buffer");

		backBuffer = nullptr;
		m_RenderTargetView = nullptr;

		hr = m_SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0u);
		ASSERTM(hr == S_OK, "[DirectX] Failed to resize buffer");

		CreateRenderTargetView();

		DXGI_MODE_DESC desc;
		desc.Width = width;
		desc.Height = height;

		m_DepthStencilBuffer = nullptr;
		m_DepthStencilView = nullptr;
		m_DepthStencilState = nullptr;

		CreateDepthBuffer(desc);
	}

	void DirectXContext::InitializeDirectX(DXGI_MODE_DESC bufferDesc) {
		DXGI_SWAP_CHAIN_DESC scd;

		scd.BufferDesc			= bufferDesc;
		scd.SampleDesc.Count	= 1;
		scd.SampleDesc.Quality	= 0;
		scd.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount			= 1;
		scd.OutputWindow		= m_HWnd;
		scd.Windowed			= TRUE;
		scd.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD; 
		scd.Flags				= 0;

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL selectedFeatureLevels;

		UINT flags = 0;

		#ifdef ENGINE_DEBUG
		flags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		CHECK_HR(D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_HARDWARE,
			NULL, flags,
			featureLevels, (UINT)std::size(featureLevels),
			D3D11_SDK_VERSION, &scd,
			m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(),
			&selectedFeatureLevels, m_DeviceContext.GetAddressOf()), "Failed to create DirectX Device and Swap chain");

	}

	void DirectXContext::CreateRenderTargetView() {
		ComPtr<ID3D11Resource> backBuffer;
		CHECK_HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void **)backBuffer.GetAddressOf()), "Unable to get back buffer");
		CHECK_HR(m_Device->CreateRenderTargetView(backBuffer.Get(), NULL, m_RenderTargetView.GetAddressOf()), 
			"Failed to create DirectX Render Traget View");

		m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), NULL);
	}

	void DirectXContext::CreateRasterizerState() {
		D3D11_RASTERIZER_DESC rd;
		rd.FillMode					= D3D11_FILL_SOLID;
		rd.CullMode					= D3D11_CULL_NONE;
		rd.FrontCounterClockwise	= FALSE;
		rd.DepthBias				= 0;
		rd.DepthBiasClamp			= 0.0f;
		rd.SlopeScaledDepthBias		= 0.0f;
		rd.DepthClipEnable			= FALSE;
		rd.ScissorEnable			= FALSE;
		rd.MultisampleEnable		= FALSE;
		rd.AntialiasedLineEnable	= FALSE;

		CHECK_HR(m_Device->CreateRasterizerState(&rd, m_RasterizerState.GetAddressOf()), "Failed to create Rasterizer state");

		m_DeviceContext->RSSetState(m_RasterizerState.Get());
	}

	void DirectXContext::CreateDepthBuffer(DXGI_MODE_DESC bufferDesc) {
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));

		dsd.DepthEnable						= TRUE;
		dsd.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc						= D3D11_COMPARISON_LESS_EQUAL;
		dsd.StencilEnable					= FALSE;

		CHECK_HR(m_Device->CreateDepthStencilState(&dsd, m_DepthStencilState.GetAddressOf()), "Failed to create Depth Stencil State");

		m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1u);

		//Move DepthStencilState to render state

		D3D11_TEXTURE2D_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));

		dbd.Width					= bufferDesc.Width;
		dbd.Height					= bufferDesc.Height;
		dbd.MipLevels				= 1;
		dbd.ArraySize				= 1;
		dbd.Format					= DXGI_FORMAT_D32_FLOAT;
		dbd.SampleDesc.Count		= 1;
		dbd.SampleDesc.Quality		= 0;
		dbd.Usage					= D3D11_USAGE_DEFAULT;
		dbd.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		dbd.CPUAccessFlags			= 0;
		dbd.MiscFlags				= 0;

		CHECK_HR(m_Device->CreateTexture2D(&dbd, NULL, m_DepthStencilBuffer.GetAddressOf()), "Failed to create Depth Stencil Texture2D");

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format					= DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice		= 0;

		CHECK_HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvd, m_DepthStencilView.GetAddressOf()), 
			"Failed to create Depth Stencil View");
		
		m_DeviceContext->OMSetRenderTargets(1u, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	}

	void DirectXContext::LogInfo() {
		
		LOG_INFO("DirectX Info:");
		
		ComPtr<IDXGIFactory> factory;
		ComPtr<IDXGIAdapter> adapter;

		HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(factory.GetAddressOf()));

		if (FAILED(hr)) {
			LOG_ERROR("Unable to create IDXGIFactory");
			return;
		}
		
		hr = factory->EnumAdapters(0u, adapter.GetAddressOf());
		if (FAILED(hr)) {
			LOG_ERROR("Unable to get default adapter");
			return;
		}

		DXGI_ADAPTER_DESC desc = {};

		hr = adapter->GetDesc(&desc);
		if (FAILED(hr)) {
			LOG_ERROR("Unable to get adapter description");
			return;
		}

		LOG_INFO("\tVendor: %s", static_cast<pvchar *>(_bstr_t(desc.Description)));
		LOG_INFO("\tMemory: %uMB", static_cast<pvuint>(desc.DedicatedVideoMemory) / (1024u * 1024u));

		D3D_FEATURE_LEVEL featureLevel = m_Device->GetFeatureLevel();

		pvstring featureLevelStr;

		switch (featureLevel) {
			case D3D_FEATURE_LEVEL_9_1 : featureLevelStr = "Feature Level 9.1"; break;
			case D3D_FEATURE_LEVEL_9_2 : featureLevelStr = "Feature Level 9.2"; break;
			case D3D_FEATURE_LEVEL_9_3 : featureLevelStr = "Feature Level 9.3"; break;
			case D3D_FEATURE_LEVEL_10_0: featureLevelStr = "Feature Level 10.0"; break;
			case D3D_FEATURE_LEVEL_10_1: featureLevelStr = "Feature Level 10.1"; break;
			case D3D_FEATURE_LEVEL_11_0: featureLevelStr = "Feature Level 11.0"; break;
			case D3D_FEATURE_LEVEL_11_1: featureLevelStr = "Feature Level 11.1"; break;
			case D3D_FEATURE_LEVEL_12_0: featureLevelStr = "Feature Level 12.0"; break;
			case D3D_FEATURE_LEVEL_12_1: featureLevelStr = "Feature Level 12.1"; break;
		default:
			ASSERTM(false, "Invalid DirectX Feature Level");
			break;
		}

		LOG_INFO("\tVersion: %s", featureLevelStr.c_str());

	}

}

#endif