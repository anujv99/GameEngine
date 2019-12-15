#pragma once

#ifdef ENGINE_DIRECTX

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "graphics/graphicscontext.h"

namespace prev {

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class DirectXContext : public GraphicsContext {
	public:
		DirectXContext();
		~DirectXContext();
	public:
		virtual void CreateContext(pvptr rawWindowPtr) override;
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		inline ComPtr<ID3D11Device> GetDevice() { return m_Device; }
		inline ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_DeviceContext; }
	private:
		void InitializeDirectX(DXGI_MODE_DESC bufferDesc);
		void CreateRenderTargetView();
		void CreateRasterizerState();
		void CreateDepthBuffer(DXGI_MODE_DESC bufferDesc);
	private:
		::GLFWwindow * m_WindowPtr;
		::HWND m_HWnd;
	private:
		ComPtr<IDXGISwapChain>				m_SwapChain;
		ComPtr<ID3D11Device>				m_Device;
		ComPtr<ID3D11DeviceContext>			m_DeviceContext;
		ComPtr<ID3D11RenderTargetView>		m_RenderTargetView;
		ComPtr<ID3D11RasterizerState>		m_RasterizerState;
		ComPtr<ID3D11Texture2D>				m_DepthStencilBuffer;
		ComPtr<ID3D11DepthStencilState>		m_DepthStencilState;
		ComPtr<ID3D11DepthStencilView>		m_DepthStencilView;
	};

}

#endif