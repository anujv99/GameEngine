#include "directxframebuffer.h"

#ifdef ENGINE_DIRECTX

namespace prev {

	DEFINE_OBJECT_DIRECTX(Framebuffer, Vec2i size, TextureFormat format, FramebufferFlags flags) {
		return dynamic_cast<Framebuffer *>(new DirectXFramebuffer(size, format, flags));
	}

	DirectXFramebuffer::DirectXFramebuffer(Vec2i size, TextureFormat format, FramebufferFlags flags) : m_IsCreated(false), m_IsMultisampled(false), m_TextureSize(size) {

		m_TextureFormat = GetTextureFormat(format);

		bool msaa = false;
		unsigned int samples = 1;

		if (flags & FramebufferFlags::FRAMEBUFFER_MSAA_16x) {
			msaa = true;
			samples = 16;
		} else if (flags & FramebufferFlags::FRAMEBUFFER_MSAA_8x) {
			msaa = true;
			samples = 8;
		} else if (flags & FramebufferFlags::FRAMEBUFFER_MSAA_4x) {
			msaa = true;
			samples = 4;
		} else if (flags & FramebufferFlags::FRAMEBUFFER_MSAA_2x) {
			msaa = true;
			samples = 2;
		}

		m_IsMultisampled = msaa;

		if (msaa) {
			m_IsCreated = CreateFramebufferMSAA(size, samples);
		} else {
			m_IsCreated = CreateFramebufferNoMSAA(size);
		}
	}

	DirectXFramebuffer::~DirectXFramebuffer() {

	}

	void DirectXFramebuffer::Bind() {
		GetDeviceContext()->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthView.Get());
	}

	void DirectXFramebuffer::UnBind() {
		DirectXContext * d3dContext = dynamic_cast<DirectXContext *>(GraphicsContext::Get());
		d3dContext->BindDefaultRenderTarget();
	}

	void DirectXFramebuffer::Resolve() {
		if (!m_IsMultisampled) {
			LOG_WARN("[DirectX] Framebuffer is not multisampled. No need to call Resolve");
			return;
		}
		GetDeviceContext()->ResolveSubresource(
			m_AntialiasedTexture->GetTexture2D().Get(), 0,
			m_MultisampledTexture.Get(), 0,
			m_TextureFormat
		);
	}

	void DirectXFramebuffer::Clear() {
		static float clearColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), clearColor);
		if (m_DepthView != nullptr)
			GetDeviceContext()->ClearDepthStencilView(m_DepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DirectXFramebuffer::Clear(Vec4 color) {
		GetDeviceContext()->ClearRenderTargetView(m_RenderTargetView.Get(), &color[0]);
		if (m_DepthView != nullptr)
			GetDeviceContext()->ClearDepthStencilView(m_DepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	prevmath::Vec2i DirectXFramebuffer::GetSize() const {
		return m_TextureSize;
	}

	prev::StrongHandle<prev::Texture2D> DirectXFramebuffer::GetTexture() {
		return m_AntialiasedTexture;
	}

	prev::StrongHandle<prev::Texture2D> DirectXFramebuffer::GetDepthTexture() {
		return m_DepthTexture;
	}

	bool DirectXFramebuffer::CreateFramebufferMSAA(Vec2i size, unsigned int numSamples) {
		m_TextureSize = size;

		D3D11_TEXTURE2D_DESC textureDesc	= {};
		textureDesc.Width					= (UINT)size.x;
		textureDesc.Height					= (UINT)size.y;
		textureDesc.MipLevels				= 1;
		textureDesc.ArraySize				= 1;
		textureDesc.Format					= m_TextureFormat;
		textureDesc.SampleDesc.Count		= numSamples;
		textureDesc.SampleDesc.Quality		= 0;
		textureDesc.Usage					= D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags				= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags			= 0;
		textureDesc.MiscFlags				= 0;

		HRESULT hr = GetDevice()->CreateTexture2D(&textureDesc, nullptr, m_MultisampledTexture.GetAddressOf());

		if (FAILED(hr)) {
			LOG_ERROR("[DirectX] Failed to create framebuffer texture2d");
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
		rtvd.Format = textureDesc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		rtvd.Texture2D.MipSlice = 0;

		hr = GetDevice()->CreateRenderTargetView(
			m_MultisampledTexture.Get(), &rtvd, m_RenderTargetView.GetAddressOf()
		);

		if (FAILED(hr)) {
			LOG_ERROR("[DirectX] Failed to create framebuffer render target");
			return false;
		}

		D3D11_TEXTURE2D_DESC textureDescaa		= {};
		textureDescaa.Width						= (UINT)size.x;
		textureDescaa.Height					= (UINT)size.y;
		textureDescaa.MipLevels					= 1;
		textureDescaa.ArraySize					= 1;
		textureDescaa.Format					= m_TextureFormat;
		textureDescaa.SampleDesc.Count			= 1;
		textureDescaa.SampleDesc.Quality		= 0;
		textureDescaa.Usage						= D3D11_USAGE_DEFAULT;
		textureDescaa.BindFlags					= 0;
		textureDescaa.CPUAccessFlags			= 0;
		textureDescaa.MiscFlags					= 0;

		m_AntialiasedTexture = new DirectXTexture2D(textureDescaa);

		return true;
	}

	bool DirectXFramebuffer::CreateFramebufferNoMSAA(Vec2i size) {

		D3D11_TEXTURE2D_DESC textureDesc		= {};
		textureDesc.Width						= (UINT)size.x;
		textureDesc.Height						= (UINT)size.y;
		textureDesc.MipLevels					= 1;
		textureDesc.ArraySize					= 1;
		textureDesc.Format						= m_TextureFormat;
		textureDesc.SampleDesc.Count			= 1;
		textureDesc.SampleDesc.Quality			= 0;
		textureDesc.Usage						= D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags					= D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags				= 0;
		textureDesc.MiscFlags					= 0;

		m_AntialiasedTexture = new DirectXTexture2D(textureDesc);

		m_MultisampledTexture = m_AntialiasedTexture->GetTexture2D();

		D3D11_RENDER_TARGET_VIEW_DESC rtvd		= {};
		rtvd.Format								= textureDesc.Format;
		rtvd.ViewDimension						= D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvd.Texture2D.MipSlice					= 0;

		HRESULT hr = GetDevice()->CreateRenderTargetView(
			m_MultisampledTexture.Get(), &rtvd, m_RenderTargetView.GetAddressOf()
		);

		ASSERTM(hr == S_OK, "[DirectX] Failed to create render target view");

		return true;
	}

	bool DirectXFramebuffer::CreateDepthBuffer(Vec2i size, unsigned int numSamples) {
		D3D11_TEXTURE2D_DESC dbd;
		ZeroMemory(&dbd, sizeof(dbd));

		dbd.Width					= (UINT)size.x;
		dbd.Height					= (UINT)size.y;
		dbd.MipLevels				= 1;
		dbd.ArraySize				= 1;
		dbd.Format					= DXGI_FORMAT_R32_TYPELESS;
		dbd.SampleDesc.Count		= numSamples;
		dbd.SampleDesc.Quality		= 0;
		dbd.Usage					= D3D11_USAGE_DEFAULT;
		dbd.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
		dbd.CPUAccessFlags			= 0;
		dbd.MiscFlags				= 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format						= DXGI_FORMAT_R32_FLOAT;
		if (numSamples > 1)
			srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2DMS;
		else
			srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip	= 0;
		srvDesc.Texture2D.MipLevels			= 1;

		m_DepthTexture = new DirectXTexture2D(dbd, srvDesc);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format					= DXGI_FORMAT_D32_FLOAT;
		if (numSamples > 1)
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else
			dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvd.Texture2D.MipSlice		= 0;

		HRESULT hr = GetDevice()->CreateDepthStencilView(m_DepthTexture->GetTexture2D().Get(), &dsvd, m_DepthView.GetAddressOf());
		ASSERTM(hr == S_OK, "[DirectX] Failed to create depth stencil view");

		return true;
	}

}

#endif //ENGINE_DIRECTX
