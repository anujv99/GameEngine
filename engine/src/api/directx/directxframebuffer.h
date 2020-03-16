#pragma once

#ifdef ENGINE_DIRECTX

#include "graphics/framebuffer.h"
#include "directxhelper.h"
#include "directxtexture.h"

namespace prev {

	class DirectXFramebuffer : public Framebuffer {
	public:
		DirectXFramebuffer(Vec2i size, TextureFormat format, FramebufferFlags flags);
		~DirectXFramebuffer();
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual void Resolve() override;
		virtual void Clear() override;
		virtual void Clear(Vec4 color) override;
		virtual Vec2i GetSize() const override;
		virtual StrongHandle<Texture2D> GetTexture() override;
		virtual StrongHandle<Texture2D> GetDepthTexture() override;
	private:
		bool CreateFramebufferMSAA(Vec2i size, unsigned int numSamples);
		bool CreateFramebufferNoMSAA(Vec2i size);
		bool CreateDepthBuffer(Vec2i size, unsigned int numSamples);
	private:
		ComPtr<ID3D11Texture2D> m_MultisampledTexture;
		ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
		ComPtr<ID3D11DepthStencilView> m_DepthView;

		StrongHandle<DirectXTexture2D> m_AntialiasedTexture;
		StrongHandle<DirectXTexture2D> m_DepthTexture;

		Vec2i m_TextureSize;
		bool m_IsCreated;
		bool m_IsMultisampled;
		DXGI_FORMAT m_TextureFormat;
	};

}

#endif //ENGINE_DIRECTX