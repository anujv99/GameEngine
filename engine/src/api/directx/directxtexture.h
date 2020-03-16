#pragma once

#include "graphics/texture.h"

#include "directxhelper.h"

namespace prev {

	DXGI_FORMAT GetTextureFormat(TextureFormat format);
	D3D11_FILTER GetTextureFilter(TextureFiltering filter);
	D3D11_TEXTURE_ADDRESS_MODE GetTextureAddressMode(TextureWrapping wrapping);
	pvuint GetNumComponents(TextureFormat format);

	class DirectXTexture2D : public Texture2D {
	public:
		DirectXTexture2D(Vec2i size, TextureParams params);
		DirectXTexture2D(D3D11_TEXTURE2D_DESC desc);
		DirectXTexture2D(D3D11_TEXTURE2D_DESC desc, D3D11_SHADER_RESOURCE_VIEW_DESC vDesc);
		~DirectXTexture2D();
	public:
		virtual void Bind(pvuint slot) const override;
		virtual void UnBind() const override;
		virtual void SetData(const pvubyte * pixels, pvsizet size) override;

		ComPtr<ID3D11Texture2D> GetTexture2D();
	private:
		void CreateTexture(Vec2i size, TextureParams params);
		void CreateSampleState(TextureParams params);
	private:
		ComPtr<ID3D11ShaderResourceView> m_View;
		ComPtr<ID3D11SamplerState> m_Sampler;
		pvsizet m_Width, m_Height, m_BytesPerPixel;
	};

}